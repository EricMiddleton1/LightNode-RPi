#pragma once


#include "LightNode/LightStrip.hpp"
#include "APA102.hpp"


class StripDigital : public LightStrip
{
public:
	StripDigital(size_t ledCount);

private:
	virtual void update() override;

	APA102 ledDriver;
};
