#include "LightNode/LightNode.hpp"
#include "LightPCA9685.hpp"
#include "LightAPA102.hpp"
#include "LightLB130.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

#include <yaml-cpp/yaml.h>

int main(int argc, char *argv[]) {
	std::vector<std::shared_ptr<Light>> lights;
	
	YAML::Node config = YAML::LoadFile("config.yaml");

	auto name = config["name"].as<std::string>();

	auto nodeLights = config["lights"];

	if(!nodeLights) {
		std::cerr << "[Error] No lights defined in config file" << std::endl;
		return 1;
	}

	for(const auto& nodeLight : nodeLights) {
		auto name = nodeLight["name"].as<std::string>();
		auto driver = nodeLight["driver"].as<std::string>();

		std::cout << "Creating light '" << name << "' with driver '" << driver << "'"
			<< std::endl;

		if(driver == "APA102") {
			lights.emplace_back(std::make_shared<LightAPA102>(name,
				nodeLight["size"].as<int>()));
		}
		else if(driver == "LB130") {
			lights.emplace_back(std::make_shared<LightLB130>(name,
				nodeLight["ip"].as<std::string>(), nodeLight["port"].as<int>()));
		}
		else if(driver == "PCA9685") {
			lights.emplace_back(std::make_shared<LightPCA9685>(name,
				nodeLight["channel"].as<int>()));
		}
		else {
			std::cerr << "[Error] Invalid driver: " << driver << std::endl;
		}
	}
	
	LightNode node(lights, name);

	while(true) {
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}
