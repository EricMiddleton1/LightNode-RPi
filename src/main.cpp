#include "LightNode/LightNode.hpp"
#include "LightPCA9685.hpp"
#include "LightAPA102.hpp"
#include "LightLB130.hpp"
#include "MatrixAPA102.hpp"

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
	boost::asio::io_service ioService;
	boost::asio::io_service::work ioWork(ioService);
	
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
			lights.emplace_back(std::make_shared<LightAPA102>(ioService, name,
				nodeLight["size"].as<int>()));
		}
		else if(driver == "Matrix") {
			lights.emplace_back(std::make_shared<MatrixAPA102>(ioService, name,
				nodeLight["width"].as<int>(), nodeLight["height"].as<int>()));
		}
		else if(driver == "LB130") {
			lights.emplace_back(std::make_shared<LightLB130>(ioService, name,
				nodeLight["ip"].as<std::string>(), nodeLight["port"].as<int>()));
		}
		else if(driver == "PCA9685") {
			lights.emplace_back(std::make_shared<LightPCA9685>(ioService, name,
				nodeLight["channel"].as<int>()));
		}
		else {
			std::cerr << "[Error] Invalid driver: " << driver << std::endl;
		}
	}
	
	LightNode node(lights, name);

	ioService.run();

	return 0;
}
