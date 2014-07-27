#include "Logger.hpp"

Logger* Logger::instance_ = NULL;
std::string Logger::filename_ = "";

Logger* Logger::getInstance() {
	if(filename_.empty())
		instance_ = new Logger();

	if(!instance_)
		instance_ = new Logger(filename_.c_str());

	return instance_;
}

void Logger::killInstance() {
	delete instance_;
}

void Logger::setFilename(const std::string& filename) {
	if(!instance_)
		filename_ = filename;
	else
		throw std::runtime_error("Logger: filename already set");
}
