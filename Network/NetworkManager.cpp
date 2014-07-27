/*
 * File:   networkmanager.cpp
 * Author: neo
 *
 * Created on 17 janvier 2010, 14:56
 */

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <netinet/in.h>
#include <string.h>
#include <iostream>

#include "Message.hpp"
#include "NetworkManager.hpp"

//-------------------------------------
// CONSTRUCTORS? DESTRUCTORS AND SO ON
//-------------------------------------

NetworkManager::NetworkManager() {
}

NetworkManager::~NetworkManager() {
}

//----------------
// PUBLIC METHODS
//----------------



//--------------------
// PROTECTED METHODS
//--------------------

int NetworkManager::createSocket() {
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        throw std::runtime_error("error while creating socket.");
	
    return sockfd;
}

SocketStatus NetworkManager::Receive(char* Data, std::size_t MaxSize, std::size_t& SizeReceived)
{
    // First clear the size received
    SizeReceived = 0;

    // Check parameters
    if (Data && MaxSize)
    {
        // Receive a chunk of bytes
        int Received = recv(communicationSocket, Data, static_cast<int>(MaxSize), 0);
        // Check the number of bytes received
        if (Received > 0)
        {
            SizeReceived = static_cast<std::size_t>(Received);
            return Done;
        }
        else if (Received == 0)
            return Disconnected;
        else
            throw std::runtime_error("Error with RECEIVE (protected) : Not everything received! ");
    }
    else
        throw std::invalid_argument("Cannot receive data from the network");
        	
}
SocketStatus NetworkManager::Send(const char* Data, std::size_t Size)
{
	ssize_t sent;
	// Check parameters
    if (Data && Size)
    {
        // Receive a chunk of bytes
        sent = ::send(communicationSocket, Data, static_cast<int>(Size), 0);
        // Check the number of bytes received
        if (sent > 0)
        {
            return Done;
        }
        else
            throw std::runtime_error("Error with SEND (protected) : Not everything send! ");
    }
    else
        throw std::invalid_argument("Cannot send data from the network");
}
SocketStatus NetworkManager::send(sf::Packet& PacketToSend)
{
    // Get the data to send from the packet
    std::size_t DataSize = 0;
    const char* Data = PacketToSend.OnSend(DataSize);

    // Send the packet size
    sf::Uint32 PacketSize = htonl(static_cast<unsigned long>(DataSize));
	send<sf::Uint32>(PacketSize);

    // Send the packet data
    if (PacketSize > 0)
		return Send(Data,DataSize);
    else
        return Done;
}
SocketStatus NetworkManager::receive(sf::Packet& PacketToReceive)
{
    // initialize pendingPacket
    std::vector<char> myPendingPacket;    
    myPendingPacket.clear();
    // We start by getting the size of the incoming packet
    sf::Uint32      PacketSize = 0;
    std::size_t Received   = 0;
    
	PacketSize =receiveB<sf::Uint32>();	
	PacketSize = ntohl(PacketSize);

    // Then loop until we receive all the packet data
    char Buffer[1024];
    while (myPendingPacket.size() < PacketSize)
    {
        // Receive a chunk of data
        std::size_t SizeToGet = std::min(static_cast<std::size_t>(PacketSize - myPendingPacket.size()), sizeof(Buffer));
        SocketStatus Status = Receive(Buffer, SizeToGet, Received);
        if (Status != Done)
        {
			throw std::runtime_error("Network : receive packet : receive of information didn't worked");
        }

        // Append it into the packet
        if (Received > 0)
        {
            myPendingPacket.resize(myPendingPacket.size() + Received);
            char* Begin = &myPendingPacket[0] + myPendingPacket.size() - Received;
            memcpy(Begin, Buffer, Received);
        }
    }

    // We have received all the datas : we can copy it to the user packet, and clear our internal packet
    PacketToReceive.Clear();
    if (!myPendingPacket.empty())
        PacketToReceive.OnReceive(&myPendingPacket[0], myPendingPacket.size());

    return Done;
}
