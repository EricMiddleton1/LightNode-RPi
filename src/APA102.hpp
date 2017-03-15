#pragma once

#include <vector>
#include <array>
#include <cstdint>

#include "LightNode/Color.hpp"
#include "BCM2835Lock.hpp"

class APA102
{
public:
	APA102();
	~APA102();

	void display(const std::vector<Color>& colors);

private:
	static constexpr double GAMMA = 2.3;

	BCM2835Lock bcm2835Lock;

	std::vector<char> colorToFrame(const Color& c) const;
	std::array<double, 256> gammaTable;
};
