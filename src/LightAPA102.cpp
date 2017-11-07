#include "LightAPA102.hpp"


LightAPA102::LightAPA102(const std::string& _name, size_t _ledCount)
	:	Light(_name, _ledCount) {
	
	//Clear the LEDs
	update();
}

void LightAPA102::update() {
	std::vector<Color> colors;
	colors.reserve(leds.size());

	for(const auto& led : leds) {
		colors.push_back(led.getColor());
	}

	ledDriver.display(colors);
}
