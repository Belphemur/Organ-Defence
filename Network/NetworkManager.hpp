/* 
 * File:   networkmanager.hpp
 * Author: neo
 *
 * Created on 10 février 2010, 12:45
 */

#ifndef _NETWORKMANAGER_HPP
#define	_NETWORKMANAGER_HPP

#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstddef>
#include "Message.hpp"
#include "Packet.hpp"
#include <iostream>
/**
 * This class manages the communication between clients and server.
 */
class NetworkManager {
public:
    NetworkManager();
    virtual ~NetworkManager();

    /**
     * Sends a message to the other end of network
     * @param msg The message to send
     */
    template<typename T>
    void send(const T& msg);

    /**
     * Gets a message from network NON BLOCKING RECV
     * @return The message received
     */
    template<typename T>
    T receive();
	  /**
     * Gets a message from network BLOCKING RECV
     * @return The message received
     */
    template<typename T>
    T receiveB();
    /**
     * Gets a Packet from network
     * @return Status, failed, sucess ...
     */
	SocketStatus receive(sf::Packet&);
     /**
     * Sends a Packet to the other end of network
     * @param msg The message to send
     * @return Status, failed, sucess ...
     */
     SocketStatus  send(sf::Packet&);
protected:
    /**
     * Creates a socket and returns the sockfd
     * @return The socket descriptor
     */
    virtual int createSocket();
    /**
     * Send an array of bytes to the host (must be connected first)
     */
    SocketStatus Send(const char* Data, std::size_t Size);
     /**
     *	Receive an array of bytes from the host (must be connected first).
	 *	This function will block if the socket is blocking
     */
    SocketStatus Receive(char* Data, std::size_t MaxSize, std::size_t& SizeReceived);
    bool socketEmpty;
    int communicationSocket; // This socket is used for sending and receiving data
    // int messageSockets[NB_MESSAGE_TYPES]; // This array stores the sockfd of
                                          // every socket (1 per messsage type)

};

//------------------
// TEMPLATE METHODS
//------------------

template<typename T>
void NetworkManager::send(const T& msg) {
    size_t amountSent = ::send(communicationSocket,  &msg, sizeof(T), NULL);
    
    if(amountSent < sizeof(T))
        throw std::runtime_error("Not everything sent!");

    socketEmpty = false;
}

template<typename T>
T NetworkManager::receive() {
    T ret;
    size_t amountRcv = recv(communicationSocket, &ret, sizeof(T), MSG_DONTWAIT);

    if(amountRcv == 0) {
        return T();
	}
    else if(amountRcv < sizeof(T))
        throw std::runtime_error("Not everything received! (NON BLOCKING)");
    return ret;
}
template<typename T>
T NetworkManager::receiveB() {
    T ret;
    size_t amountRcv = recv(communicationSocket, &ret, sizeof(T), NULL);

    if(amountRcv == 0) {
        return T();
	}
    else if(amountRcv < sizeof(T))
        throw std::runtime_error("Not everything received! (BLOCKING)");
    return ret;
}

#endif	/* _NETWORKMANAGER_HPP */

