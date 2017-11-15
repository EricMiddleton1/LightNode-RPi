#include "PCA9685.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cmath>

#include <bcm2835.h>


unsigned char PCA9685::ledsInUse = 0;
std::array<uint16_t, 256> PCA9685::gammaTable;
bool PCA9685::tableInitialized = false;

PCA9685::PCA9685(unsigned char _ledID)
	:	ledID{_ledID}	{

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
		bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_150);

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
	unsigned char mask = (1 << ledID);

	ledsInUse &= ~mask;

	if(ledsInUse == 0) {
		//Last one out, close i2c driver
		bcm2835_i2c_end();
	}
}

void PCA9685::setColor(const Color& _c) {
	c = _c;

	update();
}

Color PCA9685::getColor() const {
	return c;
}

void PCA9685::update() {
	std::array<char, 1+3*4> buffer;

	buffer[0] = REGISTER_LED_START + 3*4*ledID;
	uint16_t gammaVal;

	gammaVal = gammaTable[c.getRed()];
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = gammaVal & 0xFF;
	buffer[4] = gammaVal >> 8;
	
	gammaVal = gammaTable[c.getGreen()];
	buffer[5] = 0;
	buffer[6] = 0;
	buffer[7] = gammaVal & 0xFF;
	buffer[8] = gammaVal >> 8;
	
	gammaVal = gammaTable[c.getBlue()];
	buffer[9] = 0;
	buffer[10] = 0;
	buffer[11] = gammaVal & 0xFF;
	buffer[12] = gammaVal >> 8;

	if(bcm2835_i2c_write(buffer.data(), buffer.size()) != BCM2835_I2C_REASON_OK) {
		std::cout << "[Error] StripAnalog::update: Error sending I2C data"
			<< std::endl;
	}
}
