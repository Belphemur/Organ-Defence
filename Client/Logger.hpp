#ifndef _LOGGER_H
#define _LOGGER_H

#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

class Logger
{
public:
	static Logger* getInstance();
	
	static void killInstance();
	
	static void setFilename(const std::string& filename);

	bool isWorking()
	{
		if(logFile_ == NULL)
			return 0;
		return 1;
	}

	void log(string file, int line, string message = "" ) //for debugging, showing the file and the line
	{
		logFile_ << message << ".          On line: " << line << ", file: " << file << endl;
		logFile_.flush(); 
	}
	
	void log(string message = "" )			//normal message
	{
		logFile_ << message << endl;
		logFile_.flush(); 
	}
	
	void log(int message)			//int message
	{
		logFile_ << message << endl;
		logFile_.flush(); 
	}

private:
	Logger()
	{
		logFile_.open("log.txt");	//default log file
	}
	
	Logger(const char* filename)
	{
		logFile_.open(filename);	//specified log file
	}

	static Logger* instance_;
	ofstream logFile_;
	static std::string filename_;
};
#endif
