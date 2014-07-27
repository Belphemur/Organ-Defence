#include "Controller.hpp"
#include <cmath>
#include "../sdl_include.hpp"
#include "../Network/ClientNetworkManager.hpp"
#include "../Network/Message.hpp"
#include "../Network/Packet.hpp"
#include "EventListener.hpp"
#include "SceneManager.hpp"
#include "../GameEngine/consts.hh"


Controller* Controller::instance_ = NULL;

Controller* Controller::getInstance() {
	if(!instance_)
		instance_ = new Controller();
		
	return instance_;
}

void Controller::killInstance() {
	delete instance_;
}

Controller::Controller()
{}

void Controller::ListenKeyboard(EventListener* evt, ClientNetworkManager* client) 
{
	sf::Packet pak;
	//******PRESSED******
		if(evt->keyPressed(SDLK_DOWN) || evt->keyPressed(SDLK_s)) 
		{
			pak<<(sf::Uint8)3;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyPressed(SDLK_UP) || evt->keyPressed(SDLK_z)) 
		{
			pak<<(sf::Uint8)1;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyPressed(SDLK_LEFT) || evt->keyPressed(SDLK_q)) 
		{
			pak<<(sf::Uint8)2;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyPressed(SDLK_RIGHT) || evt->keyPressed(SDLK_d)) 
		{
			pak<<(sf::Uint8)0;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
	//******RELEASED******
		// Movements
		if(evt->keyReleased(SDLK_DOWN) || evt->keyReleased(SDLK_s)) 
		{
			pak<<(sf::Uint8)4;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_UP) || evt->keyReleased(SDLK_z)) 
		{
			pak<<(sf::Uint8)4;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_LEFT) || evt->keyReleased(SDLK_q)) 
		{
			pak<<(sf::Uint8)5;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_RIGHT) || evt->keyReleased(SDLK_d)) 
		{
			pak<<(sf::Uint8)5;
			client->send<sf::Uint8>((sf::Uint8)PLAYER_MOVE);
			client->send(pak);
			pak.Clear();
		}		
		// Changing weapons
		if(evt->keyReleased(SDLK_w))
		{
     		pak<<(uint8_t) 0;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_x)) 
		{
     		pak<<(uint8_t) 1;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_c)) 
		{
     		pak<<(uint8_t) 2;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_v)) 
		{
     		pak<<(uint8_t) 3;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_b)) 
		{
			pak<<(uint8_t) 4;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
		if(evt->keyReleased(SDLK_n)) 
		{
			pak<<(uint8_t) 5;
			client->send<sf::Uint8>(CHANGE_WEAPON);
			client->send(pak);
			pak.Clear();
		}
}
void Controller::ListenMouse(EventListener* evt, ClientNetworkManager* client) 
{
	sf::Packet packet;
	for(int i = 0;i < 6; i++)
		if(evt->mouseButtonPressed(i)) 
		{
			client->send<sf::Uint8>((sf::Uint8)MOUSE_CLICK);
			int x = evt->getMouseX() + SceneManager::getInstance()->getCameraX();
			int y = evt->getMouseY() + SceneManager::getInstance()->getCameraY();
			packet<<(sf::Uint8) i<<(sf::Int16)x<<(sf::Int16)y;
			client->send(packet);
		}
}
int Controller::ListenClass(EventListener* evt)
{
	if(evt->keyPressed(SDLK_s))
		return PLAYER_HEALER;
	else if(evt->keyPressed(SDLK_a))
		return PLAYER_FIGHTER;
	else if(evt->keyPressed(SDLK_c))
		return PLAYER_BUILDER;
	else
		return -1;
}
