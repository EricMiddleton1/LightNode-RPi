#include "MatrixAPA102.hpp"


MatrixAPA102::MatrixAPA102(boost::asio::io_service& ioService, const std::string& _name,
	uint8_t _width, uint8_t _height)
	:	Matrix(ioService, _name, _width, _height) {
	
	//Clear the LEDs
	update();
}

void MatrixAPA102::update() {
	std::vector<Color> colors(leds.size());

	for(int i = 0; i < leds.size(); ++i) {
		int x = i % width;
		int y = i / width;
		x = (y & 0x01) ? (width-x-1) : x;

		colors[width*y + x] = leds[i].getColor();
	}

	ledDriver.display(colors);
}
