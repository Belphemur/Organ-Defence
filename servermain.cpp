/* 
 * File:   servermain.cpp
 *
 * Created on 10 février 2010, 12:49
 * 
 */

#define TICK_TIME 50

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <signal.h>

#include "inits/common_inits.hpp"
#include "inits/server_inits.hpp"

#include "Network/Message.hpp"
#include "Processes/ProcessManager.hpp"
#include "Network/ServerNetworkManager.hpp"
#include "Client/Logger.hpp"
#include "Client/Timer.hpp"
#include "GameEngine/game.hh"

bool dispatchMessagesToGameEngine() {
	ServerNetworkManager* server = ServerNetworkManager::getInstance();
	ProcessManager* process = ProcessManager::getInstance();
	Logger* logger_ = Logger::getInstance();
	sf::Packet packet;
	
	// 1) We get the pending messages from the network
	usleep(100);
	Message header = static_cast<Message>(server->receive<sf::Uint8>());
	// ... And we forward them to the Game Engine
	
	if(header == MOUSE_CLICK || header == CHANGE_WEAPON || header == PLAYER_MOVE) {
		server->receive(packet);							
		process->sendToParent<Message>(header);
		process->sendToParent(packet);
		packet.Clear();			
	}
	else if(header == CLIENT_LEFT) {
		logger_->log("END OF SOMEONES LIFE");
		//std::cout<<"END OF MY LIFE (Child "<<(int)process->getId()<<" )"<<std::endl;
		process->sendToParent<Message>(CLIENT_LEFT);
		return false;
	}
	usleep(5);
	
	return true;
}

void forwardMessagesFromGameEngine() {
	ServerNetworkManager* server = ServerNetworkManager::getInstance();
	ProcessManager* process = ProcessManager::getInstance();
	sf::Packet packet;
	
	// 2) Then we get the messages from the Game Engine
	usleep(10);
	Message header = process->receiveFromParent<Message>();
	usleep(10);
	//while(!infoTreated &&  (process->receiveFromParent<uint8_t>() != 0)) 
	while(header != 0)
	{
		if (header == UNIT_DATA || header == UNIT_DESTROY || header == ORGAN_HP 
		 || header == PLAYER_HP || header == PLAYER_MONEY || header == UPDATE_KILLS
		 || header == NEW_WAVE || header == CHANGE_WEAPON || header == SCORE || header==PLAYER_ID)
		{
			server->send<sf::Uint8>((sf::Uint8)header);
			usleep(1);
			sf::Packet temp=process->receiveFromParent();
			server->send(temp);
		}
		else if(header == PLAYER_NAME)
		{
			std::string name;
			server->send<sf::Uint8>((sf::Uint8)PLAYER_NAME);
			usleep(1);
			sf::Packet temp=process->receiveFromParent(); // ALL info
			server->send(temp);
			sf::Uint8 id;
			temp>>id>>name;
			//std::cout<<"NOM DU JOUEUR "<<(int)id<<" = "<<name<<std::endl;
		}
		else if(header == GAME_OVER)
		{
		   server->send<sf::Uint8>((sf::Uint8)GAME_OVER);
		}
		else
			usleep(1);
		header = process->receiveFromParent<Message>();
	}
}

/*
 * Example of server main.
 */
int main(int argc, char** argv)
{
	signal(SIGINT, endOfServer);
	// 1) We initialize everything
	int nbClientMax = nbClientsMax(argc, argv);
	
	try
	{
		srand(time(NULL));
		initLogger("serverlog.txt");
		initServerNetworkManager();
		initProcessManager();
		initGameEngine();
		initTimer();
	}
	catch(std::exception& e) 
	{
		std::cerr << "One or more components could not be initialized, client can't start!" << std::endl;
		std::cerr << e.what() << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
    ServerNetworkManager* server = ServerNetworkManager::getInstance();
    ProcessManager* process = ProcessManager::getInstance();
	Logger* logger_ = Logger::getInstance();
	Game *game = Game::getInstance();
	Timer* timer_=Timer::getInstance();;
	sf::Packet packet;
	bool gameRunning;
	
    server->getReady();
	logger_->log("Server ready");
	
	// 2) ... Then  we run the server
	while(server->isAcceptingConnections())
	{
		gameRunning = false;
		// Waiting for everyone to connect ...
		if(game->getNbClient() < nbClientMax) {
			// logger_->log("Waiting for client connection ...");
			server->acceptConnection();
			game->addClient();
			logger_->log("Connection accepted");
			logger_->log("Creating Client Communicator process ...");
			process->fork();
			}
		if(game->getNbClient()==nbClientMax) 
			gameRunning = true;
		

		
		if(process->isChild()) 
		{
			/**********
				CHILD #0 ~ 3
				Client Communicator
			**********/
			logger_->log("Client Communicator created");
			
			// 1) We wait for the player to be ready
			bool GameEngineStarted=false;
			Message PlayerReady = static_cast<Message>(server->receiveB<sf::Uint8>());
			std::cout<<"CHILD :  Communicator "<<(int)process->getId()<<" created"<<std::endl;
			while(!GameEngineStarted)
			{
				usleep(10000);
				GameEngineStarted = process->receiveFromParent<bool>();
			}
			
			process->sendToParent<Message>(PlayerReady);
			server->receive(packet);
			process->sendToParent(packet);
			
			// 2) Then we launch the game
			while(GameEngineStarted) 
			{
				GameEngineStarted = dispatchMessagesToGameEngine();
				forwardMessagesFromGameEngine();
			}
			exit(EXIT_SUCCESS);
		}
		else 
		{
			
			/**********
				PARENT
				Game Engine, and everything around it
			**********/
			if(gameRunning) 
			{
				std::cout<<"GAMERUNNING: "<<std::boolalpha<<gameRunning<<std::endl;
				// 1) We tell everybody that we start the game
				logger_->log("GE:	Starting the game engine ...");
				process->sendToChildren<bool>(true);
				timer_->start();
				logger_->log("GE:	Timers started");
				while(gameRunning) 
				{
					// 2) We process the messages received from children
					gameRunning = game->processMessages();
					
					// 3) Then we update the data
					if(timer_->getTicks() >= TICK_TIME)
					{
						game->tick();
						timer_->start();
													
						// If the organ is dead, we print a gorgeous GAME OVER
						// on the screen (sending message to clients)
						if (!game->organIsAlive())
							process->sendToChildren<Message>(GAME_OVER);
						
					}
					
					// 4) And we send the changes to the children that will forward them to the clients
					std::list<Drawable> res = game->getChanges();
					for(std::list<Drawable>::iterator ii = res.begin(); ii != res.end(); ++ii) 
					{
						process->sendToChildren<Message>(UNIT_DATA);
						packet<<(sf::Uint32)ii->uid<<(sf::Uint8)ii->type<<(sf::Int16)ii->x<<(sf::Int16)ii->y<<(sf::Int16)ii->dx<<(sf::Int16)ii->dy;
						process->sendToChildren(packet);
						packet.Clear();
					}
					
					std::list<unsigned int> res2 = game->getDeletions();
					for(std::list<unsigned int>::iterator ii = res2.begin(); ii != res2.end(); ++ii) 
					{
						process->sendToChildren<Message>(UNIT_DESTROY);
						packet<<(sf::Int32)*ii;
						process->sendToChildren(packet);
						packet.Clear();
					}
				}

			}
			if(process->getNbChild()==0)
			{
				Game::killInstance();
				logger_->log("GE:	Restarting");
				game = Game::getInstance();
				std::cout<<"Ready for new game ("<<nbClientMax<<" clients)"<<std::endl;
			}
		}
    }
}
