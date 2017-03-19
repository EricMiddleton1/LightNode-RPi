#pragma once


#include <cstdint>

#include "LightNode/LightStripMatrix.hpp"
#include "APA102.hpp"

class StripMatrix : public LightStripMatrix
{
public:
	StripMatrix(uint8_t width, uint8_t height);


private:
	virtual void update() override;

	APA102 ledDriver;
};
