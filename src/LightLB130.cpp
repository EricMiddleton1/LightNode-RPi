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
		double r = std::pow(leds[0].getRed()/255., 2.2),
			g = std::pow(leds[0].getGreen()/255., 2.2),
			b = std::pow(leds[0].getBlue()/255., 2.2);
		
		Color c(255.*r + 0.5, 255.*g + 0.5, 255.*b + 0.5);

		int hue = c.getHue();
		int sat = 100.f*c.getHSVSaturation();
		int value = 100.f*c.getValue();

		std::string command = "{\"smartlife.iot.smartbulb.lightingservice\""
			":{\"transition_light_state\":{\"ignore_default\":1,\"on_off\":" + 
			std::to_string((int)(value > 0)) + ",\"color_temp\":0,"
			"\"hue\":" + std::to_string(hue) + ",\"saturation\":" + std::to_string(sat) +
			",\"brightness\":" + std::to_string(value) + ",\"transition_period\":20}}}";
		
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
