#include "StripMatrix.hpp"

#include <algorithm>
#include <iostream>

StripMatrix::StripMatrix(uint8_t _width, uint8_t _height)
	:	LightStripMatrix(_width, _height) {
	
	update();
}

void StripMatrix::update() {
	std::vector<Color> lines;

	bool odd = false;
	for(auto itr = leds.begin(); itr < leds.end(); itr += width, odd = !odd) {
		std::vector<Color> line(itr, itr + width);
		
		if(odd) {
			std::reverse(line.begin(), line.end());
		}

		lines.insert(lines.end(), line.begin(), line.end());
	}

	ledDriver.display(lines);

}
