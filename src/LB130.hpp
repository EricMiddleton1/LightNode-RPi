#pragma once

#include <boost/asio.hpp>

#include <thread>
#include <memory>
#include <string>
#include <cstdint>

#include "LightNode/LightStrip.hpp"


class LB130 : public LightStrip
{
public:
	LB130(const std::string& ipAddr, uint16_t port);
	virtual ~LB130();

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
