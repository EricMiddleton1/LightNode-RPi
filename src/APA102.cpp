#include "APA102.hpp"

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>

#include <bcm2835.h>


APA102::APA102() {
	if(!bcm2835_spi_begin()) {
		throw std::runtime_error("APA102::display: failed to initialize "
			"bcm2835 SPI library (Are you root?)");
	}

	//3.9Mhz on RPI2, 6.25 on RPI3
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);

	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	for(unsigned int i = 0; i < 256; ++i) {
		gammaTable[i] = std::pow((double)i / 256., GAMMA) * 256.;
	}
}

APA102::~APA102() {
	bcm2835_spi_end();
}

void APA102::display(const std::vector<Color>& colors) {
	std::vector<char> data = {0x00, 0x00, 0x00, 0x00};

	//Color frames
	for(auto& color : colors) {
		auto frame = colorToFrame(color);
		data.insert(data.end(), frame.begin(), frame.end());
	}

	//End frames
	for(unsigned int i = 0; i < (1 + colors.size() / 64); ++i) {
		data.push_back(0xFF);
		data.push_back(0x00);
		data.push_back(0x00);
		data.push_back(0x00);
	}

	bcm2835_spi_transfern(data.data(), data.size());
}

std::vector<char> APA102::colorToFrame(const Color& c) const {
	double r = gammaTable[c.getRed()],
		g = gammaTable[c.getGreen()],
		b = gammaTable[c.getBlue()];

	double largest = std::max(r, std::max(g, b));

	if(largest == 0) {
		return {0xFF, 0x00, 0x00, 0x00};
	}

	char brightness = std::ceil(31. * largest / 256.);

	//BGR color order
	return {0xE0 | brightness, 31*b/brightness, 31*g/brightness, 31*r/brightness};
}
