#include "StripDigital.hpp"


StripDigital::StripDigital(size_t _ledCount)
	:	LightStrip(LightStrip::Type::Digital, _ledCount) {
}

void StripDigital::update() {
	ledDriver.display(leds);
}
