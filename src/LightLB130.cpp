#include "LightLB130.hpp"

#include <iostream>
#include <numeric>
#include <stdexcept>

LightLB130::LightLB130(const std::string& _name, const std::string& _ipAddr, uint16_t _port)
	:	Light(_name, 1)
	,	ioWork(std::make_unique<boost::asio::io_service::work>(ioService))
	,	endpoint(boost::asio::ip::address::from_string(_ipAddr), _port)
	,	socket(ioService, boost::asio::ip::udp::v4())
	,	asyncThread(std::bind(&LightLB130::threadRoutine, this)) {
}

LightLB130::~LightLB130() {
	ioWork.reset();
	ioService.stop();

	asyncThread.join();
}

void LightLB130::threadRoutine() {
	ioService.run();

	std::cout << "LightLB130::threadRoutine: Exiting..." << std::endl;
}

void LightLB130::update() {
	auto ledColor = leds[0].getColor();

	double r = std::pow(ledColor.getRed()/255., 2.2),
		g = std::pow(ledColor.getGreen()/255., 2.2),
		b = std::pow(ledColor.getBlue()/255., 2.2);
									
	Color c(255.*r + 0.5, 255.*g + 0.5, 255.*b + 0.5);

	auto hue = c.getHue(), sat = c.getSat(), val = c.getVal();

	std::string command = "{\"smartlife.iot.smartbulb.lightingservice\""
		":{\"transition_light_state\":{\"ignore_default\":1,\"on_off\":" + 
		std::to_string(leds[0].isOn() && (val > 0)) + ",\"color_temp\":0,"
		"\"hue\":" + std::to_string((int)(360.f*hue/256.f)) + ",\"saturation\":" +
		std::to_string((int)(100.f*sat/256.f)) +
		",\"brightness\":" + std::to_string((int)(100.f*val/256.f)) + ",\"transition_period\":20}}}";
	
	std::cout << endpoint.address().to_string() << ":\n" << command << "\n" << std::endl;
	
	socket.send_to(boost::asio::buffer(encrypt(command)), endpoint);
}

std::vector<uint8_t> LightLB130::encrypt(const std::string& command) {
	std::vector<uint8_t> data(command.begin(), command.end());

	uint8_t key = 0xAB;
	for(auto& byte : data) {
		byte ^= key;
		key = byte;
	}

	return data;
}
