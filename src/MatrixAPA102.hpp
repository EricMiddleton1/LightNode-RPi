#pragma once

#include <boost/asio.hpp>

#include "LightNode/Matrix.hpp"
#include "APA102.hpp"


class MatrixAPA102 : public Matrix
{
public:
	MatrixAPA102(boost::asio::io_service& ioService, const std::string& name,
		uint8_t width, uint8_t height);

private:
	virtual void update() override;

	APA102 ledDriver;
};
