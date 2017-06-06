#include "LightNode/LightNode.hpp"
#include "StripAnalog.hpp"
#include "StripDigital.hpp"
#include "StripMatrix.hpp"
#include "Bulb.hpp"
#include "LB130.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " name analogCount "
			"[bulbCount bulb1IP bulb1Port ... [digitalCount [matrixWidth matrixHeight]]]"
			<< std::endl;

		return 1;
	}

	std::string name(argv[1]);
	unsigned int analogCount = std::stoi(argv[2]);

	std::vector<std::shared_ptr<LightStrip>> strips;
	for(unsigned int i = 0; i < analogCount; ++i) {
		strips.push_back(std::make_shared<StripAnalog>(i));
	}
	
	std::cout << "[Info] Starting LightNode with name '" << name << "' and "
		<< analogCount << " analog strips" << std::endl;;
	
	if(argc > 3) {
		unsigned int bulbCount = std::stoi(argv[3]);

		if(argc < (4 + 2*bulbCount)) {
			std::cout << "Error, wrong number of arguments" << std::endl;
		}

		std::cout << "and " << bulbCount << " Bulbs" << std::endl;

		for(unsigned int i = 0; i < (2*bulbCount); i += 2) {
			std::cout << "Creating bulb at " << argv[4+i] << ":" << argv[5+i] << std::endl;
			strips.push_back(std::make_shared<LB130>(argv[4+i], std::stoi(argv[5+i])));
		}
		
		unsigned int argOffset = 4 + 2*bulbCount;
		if(argc > argOffset) {
			unsigned int digitalCount = std::stoi(argv[argOffset]);
			std::cout << " and " << digitalCount << " digital LEDs" << std::endl;
			
			if(argc > (argOffset+1)) {
				if(digitalCount > 0) {
					std::cout << "\n[Error] Simultaneous use of both digital strip "
						"and matrix not supported" << std::endl;
					return 1;
				}

				unsigned char matrixWidth = std::stoi(argv[argOffset+1]),
					matrixHeight = std::stoi(argv[argOffset+2]);

				strips.push_back(std::make_shared<StripMatrix>(matrixWidth, matrixHeight));
				std::cout << " and 1 matrix (" << (int)matrixWidth << ", "
					<< (int)matrixHeight << ")";
			}
			else {
				strips.push_back(std::make_shared<StripDigital>(digitalCount));
				std::cout << " and 1 digital strip with " << digitalCount << " leds";
			}
		}
	}

	std::cout << std::endl;
	
	LightNode node(strips, name);

	while(1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
