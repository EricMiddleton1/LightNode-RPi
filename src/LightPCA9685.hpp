#pragma once

#include <string>
#include <boost/asio.hpp>

#include "LightNode/Light.hpp"
#include "PCA9685.hpp"


class LightPCA9685 : public Light
{
public:
	LightPCA9685(boost::asio::io_service& ioService, const std::string& name,
		unsigned int ledID);

private:
	virtual void update() override;

	PCA9685 ledDriver;
};
