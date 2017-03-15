#include "LightNode/LightNode.hpp"
#include "StripAnalog.hpp"
#include "StripDigital.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
	if(argc < 3 || argc > 4) {
		std::cout << "Usage: " << argv[0] << " name analogCount [digitalCount]" << std::endl;

		return 1;
	}

	std::string name(argv[1]);
	unsigned int analogCount = std::stoi(argv[2]);

	std::vector<std::shared_ptr<LightStrip>> strips;
	for(unsigned int i = 0; i < analogCount; ++i) {
		strips.push_back(std::make_shared<StripAnalog>(i));
	}
	
	std::cout << "[Info] Starting LightNode with name '" << name << "' and "
		<< analogCount << " analog strips";

	if(argc == 4) {
		unsigned int digitalCount = std::stoi(argv[3]);
		if(digitalCount == 0) {
			std::cout << "[Error] Digital count must be greater than 0 (given "
				<< std::to_string(digitalCount) << ")" << std::endl;

			return 1;
		}

		strips.push_back(std::make_shared<StripDigital>(digitalCount));

		std::cout << " and 1 digital strip with " << digitalCount << " leds";
	}

	std::cout << std::endl;
	
	LightNode node(strips, name);

	while(1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
