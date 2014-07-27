#include <cstdlib>
#include <iostream>

#include "inits/common_inits.hpp"
#include "inits/client_inits.hpp"

#include "Network/ClientNetworkManager.hpp"
#include "Network/Message.hpp"
#include "Client/EventListener.hpp"
#include "Client/Logger.hpp"
#include "Client/SceneManager.hpp"
#include "Client/Timer.hpp"
#include "Client/Controller.hpp"

#include "sdl_include.hpp"

int main(int argc, char* args[])
{
	std::string hostname = getHostname(argc, args);
	//std::cout << "Connection to " << hostname << std::endl;
	
	std::string playerName = getPlayername(argc, args);
	//std::cout << "Player name is " << playerName << std::endl;
	int classChosen=-1;
	// 1) Initializations
	try {
		initLogger("clientlog.txt");
		initSDL();
		initTTF();
		initClientNetworkManager();
		initEventListener();
		initSceneManager();
		initController();
		initTimer();
	}
	catch(std::exception& e) {
		std::cerr << "One or more components could not be initialized, client can't start!" << std::endl;
		std::cerr << e.what() << std::endl;
		
		return EXIT_FAILURE;
	}

	// 2) Getting instances of what was initialized		
	Logger* logger_ = Logger::getInstance();
	ClientNetworkManager* client_ = ClientNetworkManager::getInstance();
	EventListener* eventListener_ = EventListener::getInstance();
	SceneManager* sceneManager_ = SceneManager::getInstance();
	Controller* controller_ = Controller::getInstance();
	Timer* timer_ = Timer::getInstance();
	
	sceneManager_->loadImages();
	sceneManager_->setPlayerName(playerName);
	logger_->log("Images are loaded");
	timer_->start();
	logger_->log("Timer started");
	while(classChosen==-1)
	{
		eventListener_->startEventProcess();	//looking for user input 
		classChosen=controller_->ListenClass(eventListener_);
		sceneManager_->drawClassChoice();//scene rendering
		eventListener_->endEventProcess();		//finishing user input processing
		timer_->limitFPS();
	}
	
    // 3) We send a PLAYER_READY message to state that we are ready
    client_->connectTo(hostname);

	sf::Packet pak;
	pak << playerName<<(sf::Uint8)classChosen;
	sceneManager_->setPlayerClass(classChosen);
	
	Message msg = PLAYER_READY;
    client_->send<sf::Uint8>((sf::Uint8)msg);
	client_->send(pak);
	logger_->log("Sent a message : PLAYER_READY");
	logger_->log("Starting main loop");

	// 4) ... And we start the main loop
	while(!eventListener_->isToQuit())						//while ESC was not pressed
	{	
		timer_->start();
		eventListener_->startEventProcess();	//looking for user input 
		controller_->ListenKeyboard(eventListener_,client_);	//and sending it to the server
		controller_->ListenMouse(eventListener_,client_);
		sceneManager_->draw();//scene rendering
		eventListener_->endEventProcess();		//finishing user input processing
		timer_->limitFPS();
		sceneManager_->update(timer_->getTicks());//receive data from server and moving units
	}

	// 5) We send a message CLIENT_LEFT
	logger_->log("Quiting");
	
    client_->send<sf::Uint8>((sf::Uint8)CLIENT_LEFT);
	logger_->log("Sent a message CLIENT_LEFT");
	
	// 6) And we kill everybody (murderers !!!)
	Timer::killInstance();
	AbsTimer::killInstance();
	logger_->log("Timer is deleted");
	
	Controller::killInstance();
	logger_->log("Controller is deleted");
	
	SceneManager::killInstance();
	logger_->log("Scene Manager is cleared and deleted");
	
	EventListener::killInstance();
	logger_->log("Event listener is cleared and deleted");
	
	ClientNetworkManager::killInstance();	
	logger_->log("ClientNetworkManager is deleted");
	
	TTF_Quit(); 
	logger_->log("TTF shut down");
	SDL_Quit();
	logger_->log("SDL shut down");
	Logger::killInstance();

	return EXIT_SUCCESS;
}
