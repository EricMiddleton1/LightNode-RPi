#include "PCA9685.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <bcm2835.h>


unsigned char PCA9685::ledsInUse = 0;
unsigned char PCA9685::ledsSet = 0;
std::array<Color, PCA9685::LED_COUNT> PCA9685::leds;
std::array<uint16_t, 256> PCA9685::gammaTable;
bool PCA9685::tableInitialized = false;
std::mutex PCA9685::mutex;

PCA9685::PCA9685(unsigned char _ledID)
	:	ledID{_ledID}	{
	std::unique_lock<std::mutex> lock(mutex);

	if(ledID >= LED_COUNT) {
		throw std::runtime_error("PCA9685::PCA9685: ledID must be in range "
			"[0, " + std::to_string(LED_COUNT) + ") (given ledID=" + std::to_string(ledID) +
			")");
	}

	unsigned char mask = (1 << ledID);

	if(mask & ledsInUse) {
		throw std::runtime_error("PCA9685::PCA9685: ledID already in use ("
			+ std::to_string(ledID) + ")");
	}
	
	if(ledsInUse == 0) {
		if(!tableInitialized) {
			for(unsigned int i = 0; i < gammaTable.size(); ++i) {
				gammaTable[i] =(PWM_RESOLUTION) * std::pow((double)i / gammaTable.size(), 2.4)
					+ 0.5;
			}
			tableInitialized = true;
		}

		//First ones here, initialize i2c
		if(!bcm2835_i2c_begin()) {
			throw std::runtime_error("PCA9685::PCA9685: Failed to initialize I2C driver");
		}

		bcm2835_i2c_setSlaveAddress(I2C_ADDR);

		std::vector<char> buffer;

		//Set PWM frequency to max
		buffer = {0xFE, PRESCALAR};
		if(bcm2835_i2c_write(buffer.data(), buffer.size()) != 0) {
			throw std::runtime_error("PCA9685::PCA9685: "
				"Error setting prescalar");
		}

		//Initialize PCA9685 for AI and OUTDRV mode
		buffer = {0x00, 1 << AI_MODE, 1 << OUTDRV_MODE};
		if(bcm2835_i2c_write(buffer.data(), buffer.size()) != 0) {
			throw std::runtime_error("PCA9685::PCA9685: "
				"Error configuring PCA9585");
		}

		update();
	}
	
	ledsInUse |= mask;
}

PCA9685::~PCA9685() {
	std::unique_lock<std::mutex> lock(mutex);
	
	unsigned char mask = (1 << ledID);

	ledsInUse &= ~mask;
	ledsSet &= ~mask;

	if(ledsInUse == 0) {
		//Last one out, close i2c driver
		bcm2835_i2c_end();
	}
}

void PCA9685::setColor(const Color& c) {
	std::unique_lock<std::mutex> lock(mutex);

	leds[ledID] = c;
	ledsSet |= (1 << ledID);

	if(ledsSet == ledsInUse) {
		//All LEDs updated, send the updates to the PCA9685
		update();

		ledsSet = 0;
	}
}

Color PCA9685::getColor() const {
	std::unique_lock<std::mutex> lock(mutex);

	return leds[ledID];
}

void PCA9685::update() {
	//This function should only be called when the mutex is held

	std::vector<char> buffer;

	buffer.push_back(REGISTER_LED_START);

	for(auto& color : leds) {
		std::vector<char> values({color.getRed(), color.getGreen(),
			color.getBlue()});

		for(auto& value : values) {
			uint16_t gammaVal = gammaTable[value];
			buffer.push_back(0);
			buffer.push_back(0);
			buffer.push_back(gammaVal & 0xFF);
			buffer.push_back(gammaVal >> 8);
		}
	}

	if(bcm2835_i2c_write(buffer.data(), buffer.size()) != BCM2835_I2C_REASON_OK) {
		std::cout << "[Error] StripAnalog::update: Error sending I2C data"
			<< std::endl;
	}
}
