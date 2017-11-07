#pragma once

#include <boost/asio.hpp>

#include "LightNode/Light.hpp"
#include "APA102.hpp"


class LightAPA102 : public Light
{
public:
	LightAPA102(const std::string& name, size_t ledCount);

private:
	virtual void update() override;

	APA102 ledDriver;
};
