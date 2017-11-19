#pragma once

#include <boost/asio.hpp>

#include <thread>
#include <memory>
#include <string>
#include <cstdint>

#include "LightNode/Light.hpp"


class LightLB130 : public Light
{
public:
	LightLB130(boost::asio::io_service& ioService, const std::string& name,
		const std::string& ipAddr, uint16_t port);

private:
	virtual void update() override;
	
	static std::vector<uint8_t> encrypt(const std::string& command);

	boost::asio::ip::udp::endpoint endpoint;
	boost::asio::ip::udp::socket socket;

	unsigned int tick;
};
