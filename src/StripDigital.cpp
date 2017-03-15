#include "StripDigital.hpp"


StripDigital::StripDigital(size_t _ledCount)
	:	LightStrip(LightStrip::Type::Digital, _ledCount) {
	
	//Clear the LEDs
	update();
}

void StripDigital::update() {
	ledDriver.display(leds);
}
