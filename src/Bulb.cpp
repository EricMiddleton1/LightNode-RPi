#include "Bulb.hpp"

#include <iostream>
#include <numeric>

Bulb::Bulb(const std::string& _ipAddr, uint16_t _port)
	:	LightStrip(LightStrip::Type::Analog, 1)
	,	ioWork(std::make_unique<boost::asio::io_service::work>(ioService))
	,	endpoint(boost::asio::ip::address::from_string(_ipAddr), _port)
	,	socket(ioService)
	,	asyncThread(std::bind(&Bulb::threadRoutine, this)) {
	
	socket.connect(endpoint);
}

Bulb::~Bulb() {
	ioWork.reset();
	ioService.stop();

	asyncThread.join();
}

void Bulb::threadRoutine() {
	ioService.run();
}

void Bulb::update() {
	std::vector<uint8_t> packet;

	//Header
	packet.push_back(0x41);

	//RGB
	packet.push_back(leds[0].getRed());
	packet.push_back(leds[0].getGreen());
	packet.push_back(leds[0].getBlue());

	//White
	packet.push_back(0x00);

	//Mode (RGB)
	packet.push_back(0xF0);

	//Tail
	packet.push_back(0x0F);

	//Checksum
	packet.push_back(std::accumulate(packet.begin(), packet.end(), 0) % 256);

	try {
		socket.send(boost::asio::buffer(packet));
	}
	catch(const boost::system::error_code& ec) {
		std::cout << "[Bulb::update] socket.send exception: " << ec.message() << std::endl;
	}
}
