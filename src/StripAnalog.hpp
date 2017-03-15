#pragma once


#include "LightNode/LightStrip.hpp"
#include "PCA9685.hpp"


class StripAnalog : public LightStrip
{
public:
	StripAnalog(unsigned int ledID);

private:
	virtual void update() override;

	PCA9685 ledDriver;
};
