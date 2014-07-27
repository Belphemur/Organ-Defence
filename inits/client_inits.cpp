#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "../sdl_include.hpp"
//#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>
#define CLIENT_SDL

#include "client_inits.hpp"
#include "../Client/Logger.hpp"
#include "../Client/Timer.hpp"
#include "../Client/EventListener.hpp"
#include "../Client/SceneManager.hpp"
#include "../Client/Controller.hpp"
#include "../Network/ClientNetworkManager.hpp"

std::string getHostname(int argc, char** argv) {
	std::string hostname;

	if (argc < 2)
   	{
	   std::cout << "Warning: no hostname specified; connecting to localhost\nUse ./client HOSTNAME to connect to another server\n\n";
	   hostname = "localhost";
   	}
	else
		 hostname = argv[1];
		 
	return hostname;
}

std::string getPlayername(int argc, char** argv) {
	std::string playername;

	if (argc < 3)
   	{
		do {
			system("clear");
	   		std::cout << "Veuillez entrer un pseudo pour votre joueur : ";
	   		std::cin  >> playername;
		}
		while(playername.empty() || playername.size() > 16);
   	}
	else
		 playername = argv[2];
		 
	return playername;
}

void initSDL() {
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
	{
		Logger::getInstance()->log(__FILE__, __LINE__,SDL_GetError());
		throw std::runtime_error("Failed to initialize SDL!");
	}
	Logger::getInstance()->log("SDL initialized");
	SDL_WM_SetCaption("Organe Defense", NULL );
}

void initTTF() {
	if(TTF_Init() == -1)
	{
		Logger::getInstance()->log(__FILE__, __LINE__,"Problem while initializing SDL_TTF");
		throw std::runtime_error("Failed to initialize SDL_TTF!");
	}
	Logger::getInstance()->log("SDL_TTF initialized");
}

void initClientNetworkManager() {
	ClientNetworkManager* client_ = ClientNetworkManager::getInstance();	
	if(client_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize ClientNetworkManager");
		throw std::runtime_error("Failed to initialize Client network manager!");
	}
	Logger::getInstance()->log("ClientNetworkManager initialized");	
}

void initEventListener() {
	EventListener* eventListener_ = EventListener::getInstance();
	if(eventListener_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize Event Listener");
		throw std::runtime_error("Failed to initialize Event listener");
	}
	Logger::getInstance()->log("Event Listenner initialized");	
}

void initSceneManager() {
	SceneManager* sceneManager_ = SceneManager::getInstance();
	if(sceneManager_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize Scene Manager");
		throw std::runtime_error("Failed to initialize Scene manager");
	}
	Logger::getInstance()->log("Scene Manager initialized");	
}

void initController() {
	Controller* controller_ = Controller::getInstance();
	if(controller_ == NULL)
	{
		Logger::getInstance()->log(__FILE__, __LINE__, "Failed to initialize Controller");
		throw std::runtime_error("Failed to initialize Controller");
	}
	Logger::getInstance()->log("Controller initialized");	
}
