#include "LightNode/LightNode.hpp"
#include "StripAnalog.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " name analogCount" << std::endl;

		return 1;
	}

	std::string name(argv[1]);
	unsigned int ledCount = std::stoi(argv[2]);

	std::vector<std::shared_ptr<LightStrip>> strips;
	for(unsigned int i = 0; i < ledCount; ++i) {
		strips.push_back(std::make_shared<StripAnalog>(i));
	}

	std::cout << "[Info] Starting LightNode with name '" << name << "' and "
		<< ledCount << " analog strips" << std::endl;
	
	LightNode node(strips, name);

	while(1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
