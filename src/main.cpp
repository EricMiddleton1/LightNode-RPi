#include "LightNode/LightNode.hpp"
#include "StripAnalog.hpp"
#include "StripDigital.hpp"
#include "StripMatrix.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
	if(argc < 3 || argc == 5 || argc > 6) {
		std::cout << "Usage: " << argv[0] << " name analogCount "
			"[digitalCount [matrixWidth matrixHeight]]" << std::endl;

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
	
	if(argc > 3) {
		unsigned int digitalCount = std::stoi(argv[3]);
		
		if(argc == 6) {
			if(digitalCount > 0) {
				std::cout << "\n[Error] Simultaneous use of both digital strip "
					"and matrix not supported" << std::endl;
			}

			unsigned char matrixWidth = std::stoi(argv[4]),
				matrixHeight = std::stoi(argv[5]);

			strips.push_back(std::make_shared<StripMatrix>(matrixWidth, matrixHeight));
			std::cout << " and 1 matrix (" << (int)matrixWidth << ", "
				<< (int)matrixHeight << ")";
		}
		else {
			strips.push_back(std::make_shared<StripDigital>(digitalCount));
			std::cout << " and 1 digital strip with " << digitalCount << " leds";
		}
	}

	std::cout << std::endl;
	
	LightNode node(strips, name);

	while(1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
