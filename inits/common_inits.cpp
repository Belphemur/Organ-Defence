#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>

#include "common_inits.hpp"
#include "../Client/Logger.hpp"
#include "../Client/Timer.hpp"

void initLogger(std::string filename) {
	Logger::setFilename(filename.c_str());
	Logger* logger_ = Logger::getInstance();	
	
	if(logger_ == NULL || !logger_->isWorking())
	{
		throw std::runtime_error("Failed to Initialize Logger!");
	}
	logger_->log("Logger initialized");
}

void initTimer() {
	Timer* timer_ = Timer::getInstance();
	AbsTimer* absTimer_ = AbsTimer::getInstance();
	if((timer_ == NULL) || (absTimer_ == NULL))
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize Timers");
		throw std::runtime_error("Failed to initialize timers!");
	}
	Logger::getInstance()->log("Timers initialized");
}
