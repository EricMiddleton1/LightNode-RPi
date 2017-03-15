#include "StripAnalog.hpp"


StripAnalog::StripAnalog(unsigned int _ledID)
	:	LightStrip(LightStrip::Type::Analog, 1)
	,	ledDriver(_ledID) {
}

void StripAnalog::update() {
	ledDriver.setColor(leds[0]);
}
