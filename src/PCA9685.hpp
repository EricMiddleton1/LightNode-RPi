#pragma once

#include <array>
#include <mutex>
#include <cstdint>

#include "BCM2835Lock.hpp"
#include "LightNode/Color.hpp"

class PCA9685
{
public:
	PCA9685(unsigned char ledID);
	~PCA9685();

	PCA9685(PCA9685&) = delete;
	PCA9685& operator=(const PCA9685&) = delete;

	void setColor(const Color&);
	Color getColor() const;

private:
	static const unsigned char LED_COUNT = 5;

	static const unsigned char I2C_ADDR = 0x40;
	static const unsigned char AI_MODE = 5;
	static const unsigned char OUTDRV_MODE = 2;
	static const unsigned char REGISTER_LED_START = 0x06;
	static const unsigned char PRESCALAR = 0x03;
	static const unsigned int PWM_RESOLUTION = 4096;
	
	void update();

	Color c;
	
	static unsigned char ledsInUse;
	static std::array<uint16_t, 256> gammaTable;
	static bool tableInitialized;
	
	BCM2835Lock bcm2835Lock;

	unsigned char ledID;
};
