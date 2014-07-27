#include "../Processes/ProcessManager.hpp"
#include "../Network/ServerNetworkManager.hpp"
#include "../Network/ClientNetworkManager.hpp"
#include "../Network/Packet.hpp"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <string.h>
int main() 
{
   ProcessManager* process = ProcessManager::getInstance();
   std::cout<<std::endl<<"Server/Client and Parent/Child TESTS :"<<std::endl<<std::endl;
   process->fork();
   if(process->isParent()) 
   {
   		assert(process->getNbChild()== 1);
   		std::cout<<"P:getNbChild				passed."<<std::endl;
   		ServerNetworkManager* server = ServerNetworkManager::getInstance();
   		server->getReady();
		server->acceptConnection();
		uint8_t x= server->receiveB<uint8_t>();
		assert(x==5);
		std::cout<<"S:receiveB<uint8_t>		passed."<<std::endl;
		uint8_t y= server->receiveB<uint8_t>();
		assert(y==8);
		std::cout<<"S:receiveB<uint8_t>		passed."<<std::endl;
		server->send<uint8_t>(x+y);
		assert(server->receiveB<bool>());
   		std::cout<<"S:receiveB<bool>		passed."<<std::endl;
   		
		process->sendToChild<uint8_t>(0,10);
		process->sendToChild<uint8_t>(0,85);
		usleep(5000);
		assert(process->receiveFromChild<uint8_t>(0)==75);	
		std::cout<<"P:receiveFromChild<uint8_t>		passed."<<std::endl;
   		
		sf::Packet pak;
   		sf::Uint8 test =5;
   		sf::Uint16 t=28;

   		std::string stest="Yatta ça marche !";
   		
   		pak<<t<<test<<stest;
   		server->send(pak);
   		std::cout<<"S:send(Package)		passed."<<std::endl;
   		
   		pak.Clear();
   		pak<<(sf::Uint16)55<<(sf::Int16)-55;

   		process->sendToChild(0,pak);
   		std::cout<<"P:sendToChild(Packet)				passed."<<std::endl;
   		pak.Clear();
   		usleep(100);
   		pak=process->receiveFromChild(0);
   		sf::Int16 v;
   		sf::Uint16 u;
   		pak>>u>>v;
   		assert(u==80);
		assert(v==-90);
		std::cout<<"C:receiveFromChild(Packet)			passed."<<std::endl;
		process->childKilled(0);
   		assert(process->firstChild() == process->lastChild());
   		assert(process->getNbChild() == 0);
   		
   		std::cout<<"P:childKilled				passed."<<std::endl;
   		ServerNetworkManager::killInstance();
   		ProcessManager::killInstance();
		exit(EXIT_SUCCESS);
   }
   else 
   {
		assert(process->isChild());
		std::cout<<"C:isChild			passed."<<std::endl;
		assert(process->getId()== 0);
		std::cout<<"C:getId				passed."<<std::endl;
		ClientNetworkManager* client = ClientNetworkManager::getInstance();
		client->connectTo("localhost");
		client->send<uint8_t>(5);
		client->send<uint8_t>(8);
		
		uint8_t somme=client->receiveB<uint8_t>();
		client->send<bool>(true);
		assert(somme==13);
		std::cout<<"C:receiveB<uint8_t>		passed."<<std::endl;

		
		usleep(150);
		uint8_t x= process->receiveFromParent<uint8_t>();
		assert(x==10);
		std::cout<<"C:receiveFromParent<uint8_t>		passed."<<std::endl;
		usleep(150);
		uint8_t y= process->receiveFromParent<uint8_t>();
		assert(y==85);
		std::cout<<"C:receiveFromParent<uint8_t>		passed."<<std::endl;
		process->sendToParent<uint8_t>(y-x);
		
		sf::Packet pak;	
		client->receive(pak);
		sf::Uint16  t;
		sf::Uint8 test;
		std::string stest;
		
		pak>>t>>test>>stest;
		assert(t==28);
		assert(test==5);
		assert(stest=="Yatta ça marche !");
		std::cout<<"C:receive(Packet)	passed."<<std::endl;
		
		usleep(100);
		pak.Clear();
		pak=process->receiveFromParent();
		sf::Int16 u;
		pak>>t>>u;
		assert(t==55);
		assert(u==-55);
		std::cout<<"C:receiveFromParent(Packet)			passed."<<std::endl;
		pak.Clear();
		pak<<(sf::Uint16)80<<(sf::Int16)-90;
		
		process->sendToParent(pak);
		std::cout<<"C:sendToParent(Packet)			passed."<<std::endl;
		ClientNetworkManager::killInstance();
		exit(EXIT_SUCCESS);
   }
}

