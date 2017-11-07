#include "LightPCA9685.hpp"


LightPCA9685::LightPCA9685(const std::string& _name,
	unsigned int _ledID)
	:	Light(_name, 1)
	,	ledDriver(_ledID) {
}

void LightPCA9685::update() {
	ledDriver.setColor(leds[0].getColor());
}
