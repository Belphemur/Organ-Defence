#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "server_inits.hpp"
#include "../Client/Logger.hpp"
#include "../Client/Timer.hpp"
#include "../Network/ServerNetworkManager.hpp"
#include "../GameEngine/game.hh"
#include "../Processes/ProcessManager.hpp"



int nbClientsMax(int argc, char** argv) {
	if (argc < 3)
   	{
		std::cerr << "usage: "<<argv[0]<<" [-n] NUMBER_OF_CLIENTS"<<std::endl<<"	0 < NUMBER_OF_CLIENTS < 5"<<std::endl; 
		return EXIT_FAILURE;
   	}
   	if(strcmp(argv[1],"-n")!=0)
	{
		std::cerr << "usage: "<<argv[0]<<" [-n] NUMBER_OF_CLIENTS"<<std::endl<<"	0 < NUMBER_OF_CLIENTS < 5"<<std::endl; 
		return EXIT_FAILURE;
	}
	std::string argument=argv[2];
	int nbClientMax = std::atoi(argument.c_str());
	if(nbClientMax > 4 || nbClientMax <1 )
	{
		std::cerr << "usage: "<<argv[0]<<" [-n] NUMBER_OF_CLIENTS"<<std::endl<<"	0 < NUMBER_OF_CLIENTS < 5"<<std::endl; 
		return EXIT_FAILURE;
	}
	
	return nbClientMax;
}

void initServerNetworkManager() {
	ServerNetworkManager* server_ = ServerNetworkManager::getInstance();	
	if(server_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize ServerNetworkManager");
		throw std::runtime_error("Failed to initialize Server network manager!");
	}
	Logger::getInstance()->log("ServerNetworkManager initialized");	
}

void initAbsTimer() 
{
	AbsTimer* absTimer_ = AbsTimer::getInstance();
	if(absTimer_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize AbsTimer");
		throw std::runtime_error("Failed to initialize asbTimer!");
	}
	Logger::getInstance()->log("AbsTimer initialized");
}
void initProcessManager()
{
	ProcessManager* process = ProcessManager::getInstance();
	if(process == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize ProcessManager");
		throw std::runtime_error("Failed to initialize process!");
	}
	Logger::getInstance()->log("ProcessManager initialized");
}
void initGameEngine()
{
    srand(time(NULL));
	Game *game = Game::getInstance();
	if(game == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize GameEngine");
		throw std::runtime_error("Failed to initialize game!");
	}
	Logger::getInstance()->log("GameEngine initialized");
}
void endOfServer(int param)
{
	Logger *logger_=Logger::getInstance();
	Game::killInstance();
	logger_->log("GE:	Quitting");
	Timer::killInstance();
	logger_->log("Timer killed");
    ProcessManager::killInstance();
	logger_->log("Process killed");
    ServerNetworkManager::killInstance();
	logger_->log("Server killed");
	Logger::killInstance();
	std::cout<<std::endl<<"Server Stopped"<<std::endl;
	exit(param);
}
