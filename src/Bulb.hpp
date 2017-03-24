#pragma once

#include <boost/asio.hpp>

#include <thread>
#include <memory>
#include <string>
#include <cstdint>

#include "LightNode/LightStrip.hpp"


class Bulb : public LightStrip
{
public:
	Bulb(const std::string& ipAddr, uint16_t port);
	~Bulb();

private:
	virtual void update() override;
	
	void threadRoutine();
	
	boost::asio::io_service ioService;
	std::unique_ptr<boost::asio::io_service::work> ioWork;
	
	boost::asio::ip::tcp::endpoint endpoint;
	boost::asio::ip::tcp::socket socket;

	std::thread asyncThread;
};
