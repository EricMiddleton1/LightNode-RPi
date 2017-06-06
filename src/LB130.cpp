#include "LB130.hpp"

#include <iostream>
#include <numeric>
#include <stdexcept>

LB130::LB130(const std::string& _ipAddr, uint16_t _port)
	:	LightStrip(LightStrip::Type::Analog, 1)
	,	ioWork(std::make_unique<boost::asio::io_service::work>(ioService))
	,	endpoint(boost::asio::ip::address::from_string(_ipAddr), _port)
	,	socket(ioService, boost::asio::ip::udp::v4())
	,	asyncThread(std::bind(&LB130::threadRoutine, this)) {
}

LB130::~LB130() {
	std::cout << "LB130::~LB130" << std::endl;

	ioWork.reset();
	ioService.stop();

	asyncThread.join();
}

void LB130::threadRoutine() {
	ioService.run();

	std::cout << "LB130::threadRoutine: Exiting..." << std::endl;
}

void LB130::update() {
	static bool tick = false;

	tick = !tick;

	//if(tick) {

		double r = std::pow(leds[0].getRed()/255., 2.2),
			g = std::pow(leds[0].getGreen()/255., 2.2),
			b = std::pow(leds[0].getBlue()/255., 2.2);
		
		Color c(255.*r + 0.5, 255.*g + 0.5, 255.*b + 0.5);

		float hue = c.getHue();
		float sat = 100.f*c.getHSVSaturation();
		float value = 100.f*c.getValue();

		std::string command = "{\"smartlife.iot.smartbulb.lightingservice\""
			":{\"transition_light_state\":{\"ignore_default\":1,\"on_off\":" + 
			std::to_string((int)(value > 5)) + ",\"color_temp\":0,"
			"\"hue\":" + std::to_string(hue) + ",\"saturation\":" + std::to_string(sat) +
			",\"brightness\":" + std::to_string(value) + ",\"transition_period\":10}}}";
		
		//std::cout << command << std::endl;
		
		socket.send_to(boost::asio::buffer(encrypt(command)), endpoint);
	//}
}

std::vector<uint8_t> LB130::encrypt(const std::string& command) {
	std::vector<uint8_t> data(command.begin(), command.end());

	uint8_t key = 0xAB;
	for(auto& byte : data) {
		byte ^= key;
		key = byte;
	}

	return data;
}
