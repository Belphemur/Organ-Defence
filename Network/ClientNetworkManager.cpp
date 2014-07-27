/*
 * File:   clientnetworkmanager.cpp
 * Author: neo
 *
 * Created on 17 janvier 2010, 14:57
 */

#include <cstdlib>
#include <strings.h> // Needed for bzero
#include <string>
#include <stdexcept>
#include <iostream>

#include <netdb.h>
#include <sys/socket.h>

#include "ClientNetworkManager.hpp"

ClientNetworkManager* ClientNetworkManager::instance_ = NULL;

//--------------------------------------
// CONSTRUCTORS, DESTRUCTORS AND SO ON
//--------------------------------------

ClientNetworkManager* ClientNetworkManager::getInstance() {
    if(!instance_)
        instance_ = new ClientNetworkManager();

    return instance_;
}

void ClientNetworkManager::killInstance() {
    if(instance_)
        delete instance_;
}

ClientNetworkManager::ClientNetworkManager() : connected(false) {
}

ClientNetworkManager::~ClientNetworkManager() {
}

//-----------------
// PUBLIC METHODS
//-----------------

void ClientNetworkManager::connectTo(std::string hostname) {
    // 1) We check whether the hostname exists
    if(hostname.empty())
        throw std::invalid_argument("connection to server: no hostname given.");

    // 2) If so, we get server infos
    struct sockaddr_in serverAddress = getServerAddress(hostname, DEFAULT_PORT);

    // 3) Then, we create the socket
    communicationSocket = createSocket();

    // 4) Now we can connect to the server :)
    int res = connect(communicationSocket, (struct sockaddr*) &serverAddress, sizeof(struct sockaddr));
    if(res < 0)
        throw std::runtime_error("Error while trying to connect to server: connection failed.");

    connected = true;
}

//--------------------
// PROTECTED METHODS
//--------------------

/**
 * Creates the structure from the hostname given in parameter
 * @param hostname string The server name
 * @return sockaddr_in the struct with everything in it.
 */
struct sockaddr_in ClientNetworkManager::getServerAddress(std::string hostname, unsigned short port) {
    struct hostent* serverInfos = gethostbyname(hostname.c_str());

    if(serverInfos == NULL)
        throw std::runtime_error("Connecting to server: Invalid hostname");

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr = *((struct in_addr*) serverInfos->h_addr);
    serverAddress.sin_port = htons(port);
    bzero(serverAddress.sin_zero, 8);

    return serverAddress;
}
