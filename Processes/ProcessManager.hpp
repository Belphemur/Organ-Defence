/* 
 * File:   ProcessManager.hpp
 * Author: Antoine Aflalo
 *
 * Created on 10 février 2010, 12:46
 */

#ifndef _PROCESSMANAGER_HPP
#define	_PROCESSMANAGER_HPP
#include <map>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdexcept>
#include <string.h>
#include "../Network/Packet.hpp"

#include "Pipe.hpp"


class ProcessManager {
public:
    static ProcessManager* getInstance();
    static void killInstance();
	/**
	 * Create a fork of the processus.
	 */
    virtual void fork();
	/**
	 * Parent process
	 * @return true if parent
	 */
    virtual bool isParent();
	/**
	 * Child process
	 * @return true if child
	 */
    virtual bool isChild();
	//Send
	/**
	 * Send info to the parent
	 * @param info sended
	 * @return none
	 */
	template<typename T> void sendToParent(T);
	/**
	 * Send info to the child
	 * @param id of the child
	 * @param info sended
	 * @return none
	 */
	template<typename T> void sendToChild(uint8_t,T);
	/**
	 * Send info to the children
	 * @param info sended
	 * @return none
	 */
	template<typename T> void sendToChildren(T);
	/**
	 * Send Packet to the parent
	 * @param Packet sended
	 * @return none
	 */
	void sendToParent(sf::Packet);
	/**
	 * Send Packet to the child
	 * @param id of the child
	 * @param Packet sended
	 * @return none
	 */
	void sendToChild(uint8_t,sf::Packet);
	/**
	 * Send Packet to the children
	 * @param Packet sended
	 * @return none
	 */
	void sendToChildren(sf::Packet);
	//Receive
	/**
	 * Receive information from the child
	 * @param id of the child
	 * @return info received
	 */
	template<typename T> T receiveFromChild(uint8_t);
	/**
	 * Receive info from the parent
	 * @param none
	 * @return info receved
	 */
	template<typename T> T receiveFromParent();
	/**
	 * Receive Packet from the child
	 * @param id of the child
	 * @return Packet receved
	 */
	sf::Packet receiveFromChild(uint8_t);
	/**
	 * Receive Packet from the parent
	 * @param none
	 * @return Packet receved
	 */
	sf::Packet receiveFromParent();
	//Pending message
	/*
	 * Check for msg using the select initialed with initPendingMessage()
	 */
	//void getPendingMessages();
	/*
	 * Check if there is a msg from the chose pipe
	 * @return true if there is a msg from the chosen pipe else false
	 */
	//bool hasPendingMessage(uint8_t);
	/*
	 * Intitialize the FD
	 * @return none
	 */
	//void initPendingMessage();
	//Special PARENT
	/**
	 * Decrement the counter of child and remove the ID from the livingChildren
	 */
	void childKilled(uint8_t);
	/**
	 * Get the iterator to the first child
	 */
	const std::vector<uint8_t>::iterator firstChild();
	/**
	 * Get the iterator to the last child
	 */
	const std::vector<uint8_t>::iterator lastChild();
	/**
	 * Getter of nbChild (for a child, represent his ID)
	 * @return id for the child and number of threads for the parent
	 */
	uint8_t getNbChild() const;
	/**
	 * Get number of child created
	 */
	uint8_t getId() const;
protected:
	/**
	 * Check if the child exist (not dead)
	 */
    bool isChildAlive(uint8_t);
private:
    ProcessManager();
    virtual ~ProcessManager();

    static ProcessManager* instance_;
	
    pid_t childPid;
	uint8_t nbChild;
	fd_set readFds;
	//int max;
	std::vector<uint8_t>livingChildren; //Id of the livings Children
	std::map<uint8_t,Pipe> vectChildParent; //To parent
	std::map<uint8_t,Pipe> vectParentChild; //To child
};



template<typename T>
void ProcessManager::sendToChild(uint8_t id, T info)
{
	if(!(this->isChildAlive(id)))
		throw std::runtime_error("Child don't exist (sendToChild)");
		
	size_t amountWritten;
	amountWritten = vectParentChild[id].write<T>(info);
	
	if(amountWritten < sizeof(T))
		throw std::runtime_error("Not everything written ! (sendToChild)");
}


template<typename T>
void ProcessManager::sendToParent(T info)
{
	size_t amountWriten;
	amountWriten=vectChildParent[nbChild].write<T>(info);
	
	if(amountWriten< sizeof(T))
		throw std::runtime_error("Not everything written ! (sendToParent)");
}


template<typename T>
void ProcessManager::sendToChildren(T info)
{
	for(std::vector<uint8_t>::iterator it=livingChildren.begin();it != livingChildren.end(); it++)
	{
		this->sendToChild<T>(*it,info);
	}
}


template<typename T>
T ProcessManager::receiveFromParent()
{
	ssize_t amountRead;
	T info;
	amountRead=vectParentChild[nbChild].read<T>(info);
	if(amountRead==-1)
		return T();
	return info;
}

template<typename T>
T ProcessManager::receiveFromChild(uint8_t id)
{
	if(!(this->isChildAlive(id)))
		throw std::runtime_error("Child don't exist (receiveFromChild)");
	T info;
	ssize_t amountRead;
	amountRead = vectChildParent[id].read<T>(info);
	if(amountRead==-1)
		return T();
	return info;
}
#endif	/* _ProcessManager_HPP */
