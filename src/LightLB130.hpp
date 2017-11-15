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
	LightLB130(boost::asio::io_service& ioService, const std::string& name, const std::string& ipAddr, uint16_t port);
	virtual ~LightLB130();

private:
	virtual void update() override;
	
	void threadRoutine();
	
	static std::vector<uint8_t> encrypt(const std::string& command);

	boost::asio::io_service ioService;
	std::unique_ptr<boost::asio::io_service::work> ioWork;
	
	boost::asio::ip::udp::endpoint endpoint;
	boost::asio::ip::udp::socket socket;

	std::thread asyncThread;
};
