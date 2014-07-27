/*
 * File:   ProcessManager.cpp
 * Author: Antoine Aflalo
 *
 * Created on 17 janvier 2010, 14:57
 */


#include <sys/wait.h>
#include <signal.h>
#include <algorithm>
#include <iostream>
#include <map>

#include "Pipe.hpp"
#include "ProcessManager.hpp"
/**
 * Kill child zombie
 * @param i dont know
 * @return none.
 */
void sigChildHandler(int s)
{
   /*Kills all the zombie processes*/
   while(waitpid(-1, &s, WNOHANG) > 0) ;

}

ProcessManager* ProcessManager::instance_ = NULL;

//--------------------------------------
// CONSTRUCTORS, DESTRUCTORS AND SO ON
//--------------------------------------

ProcessManager* ProcessManager::getInstance() {
    if(!instance_)
        instance_ = new ProcessManager;
	return instance_;
}

void ProcessManager::killInstance() {
        delete instance_;
}

ProcessManager::ProcessManager():nbChild(0) {
}

ProcessManager::~ProcessManager() {
	for (std::vector<uint8_t>::iterator i = livingChildren.begin(); i!=livingChildren.end();++i)
	{
		vectChildParent[(*i)].clear();
		vectParentChild[(*i)].clear();
	}
	vectChildParent.clear();
	vectParentChild.clear();
	livingChildren.clear();
}

//-----------------
// PUBLIC METHODS
//-----------------

void ProcessManager::fork() {
	//Signal kill child (zombie)
	struct sigaction sa;
	sa.sa_handler=	sigChildHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_RESTART;
	if(sigaction(SIGCHLD, &sa,NULL)==-1)
		throw std::runtime_error("zombie killer failed");

	Pipe child,parent;
	vectChildParent[nbChild]=child;
	vectParentChild[nbChild]=parent;

	if (vectChildParent[nbChild].init() == -1 ||vectParentChild[nbChild].init() == -1)
	{
    	throw std::runtime_error("pipe failed");
	}
	childPid = ::fork();
	if(childPid==-1)
		throw std::runtime_error("fork failed");

	if (childPid==0) //child
	{
		if(nbChild > 0)
		{
			for(uint8_t i = 0; i< nbChild;++i)
			{
				vectParentChild[i].clear();
				vectChildParent[i].clear();
			}
		}
		vectParentChild[nbChild].close(WRITE);
		vectChildParent[nbChild].close(READ);
		//Deblock read pipe
		vectParentChild[nbChild].deblock(READ);
	}
	else
	{
		vectParentChild[nbChild].close(READ);
		vectChildParent[nbChild].close(WRITE);
		livingChildren.push_back(nbChild);
		//Deblock read pipe
		vectChildParent[nbChild].deblock(READ);
		nbChild++;
	}

}

bool ProcessManager::isParent() {
    return (!this->isChild());
}

bool ProcessManager::isChild() {
    return (childPid == 0);
}



uint8_t ProcessManager::getId() const
{
	return nbChild;
}

/*void ProcessManager::initPendingMessage()
{
	if(this->isChild())
		throw std::runtime_error("Child try to getPendingMessages");

	FD_ZERO(&readFds);
	max=*(max_element(vectChildParent.begin(),vectChildParent.end()))[0];
	for(uint8_t i=0; i<nbChild;++i)
		FD_SET(vectChildParent[i][0], &readFds);
}

void ProcessManager::getPendingMessages()
{
	select(max+1,&readFds,NULL,NULL, NULL);
}

bool ProcessManager::hasPendingMessage(uint8_t id)
{
	if(!(this->isChildAlive(id)))
		throw std::runtime_error("Child don't exist (hasPendingMessage)");
	return (FD_ISSET(vectChildParent[id][0], &readFds));
}
*/
 void ProcessManager::childKilled(uint8_t id)
 {
	if(this->isParent())
	{
	 	--nbChild;
	 	livingChildren.erase(std::find(livingChildren.begin(), livingChildren.end(), id));
		vectChildParent[id].clear();
		vectParentChild[id].clear();
		
		vectChildParent.erase(id);
		vectParentChild.erase(id);
	}
	else
		throw std::runtime_error("Parent Method (childKilled)");
}

uint8_t ProcessManager::getNbChild() const
{
	return nbChild;
}

const std::vector<uint8_t>::iterator ProcessManager::firstChild() 
{
	return livingChildren.begin();
}

const std::vector<uint8_t>::iterator ProcessManager::lastChild()
{
	return livingChildren.end();
}

bool ProcessManager::isChildAlive(uint8_t id)
{
	return (vectChildParent.find(id)!=vectChildParent.end());
}

//packet



void ProcessManager::sendToChild(uint8_t id, sf::Packet PacketToSend)
{
	vectParentChild[id].writePacket(PacketToSend);
}


void ProcessManager::sendToParent(sf::Packet PacketToSend)
{
	vectChildParent[nbChild].writePacket(PacketToSend);
}


void ProcessManager::sendToChildren(sf::Packet pak)
{
	for(std::vector<uint8_t>::iterator it=livingChildren.begin();it != livingChildren.end(); it++)
	{
		this->sendToChild(*it,pak);
	}
}

sf::Packet  ProcessManager::receiveFromParent()
{
	sf::Packet received;
	received.Clear();
	vectParentChild[nbChild].readPacket(received);
	return received; 
}

sf::Packet ProcessManager::receiveFromChild(uint8_t id)
{
	if(!(this->isChildAlive(id)))
		throw std::runtime_error("Child don't exist (receiveFromChild PACKET)");
		
	sf::Packet received;
	received.Clear();
	vectChildParent[id].readPacket(received);
	return received; 	
        
}
