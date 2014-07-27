/*
 * File:   servernetworkmanager.cpp
 * Author: neo
 *
 * Created on 17 janvier 2010, 14:57
 */

#include <cstdlib>
#include <strings.h> // Needed for bzero()
#include <stdexcept>
#include <algorithm>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h> 

#include "ServerNetworkManager.hpp"

ServerNetworkManager* ServerNetworkManager::instance_ = NULL;

//--------------------------------------
// CONSTRUCTORS, DESTRUCTORS AND SO ON
//--------------------------------------

ServerNetworkManager* ServerNetworkManager::getInstance() {
    if(!instance_)
        instance_ = new ServerNetworkManager();

    return instance_;
}

void ServerNetworkManager::killInstance() {
    if(instance_)
        delete instance_;
}

ServerNetworkManager::ServerNetworkManager() : ready(false) {
}

ServerNetworkManager::~ServerNetworkManager() {
}

//--------------------------------
// PUBLIC METHODS : GETTING READY
//--------------------------------

void ServerNetworkManager::getReady() {
    // 1) First, we create a new socket to listen to new connections
    listeningSocket = createSocket();

    // 2) Then we create the struct giving information about our address
    struct sockaddr_in myAddress = getMyAddress(DEFAULT_PORT);

    // 3) Then, we bind the socket
    int res = bind(listeningSocket, (struct sockaddr*) &myAddress, sizeof(struct sockaddr));
    if(res < 0)
        throw std::runtime_error("Error while getting ready: bind failed.");

    // 4) Now, we start listening on the socket
    res = listen(listeningSocket, BACKLOG_SIZE);
    if(res < 0)
        throw std::runtime_error("Error while getting ready: listen failed.");

    // ... And we're ready to accept connections :)
    ready = true;
}

//---------------------------------------
// PUBLIC METHODS : MANAGING CONNECTIONS
//---------------------------------------

void ServerNetworkManager::acceptConnection() {
    // The clientAddress may be useful so we save it
    struct sockaddr_in clientAddress;
    socklen_t addressLength = sizeof(struct sockaddr_in);

    communicationSocket = accept(listeningSocket, (struct sockaddr*) &clientAddress, &addressLength);
    if(communicationSocket < 0)
        throw std::runtime_error("Error while accepting connections: accept failed.");
}

bool ServerNetworkManager::isAcceptingConnections() {
    return ready;
}

void ServerNetworkManager::stopAcceptingConnections() {
    ready = false;
}

//------------------------------------
// PUBLIC METHODS : MANAGING MESSAGES
//------------------------------------

bool ServerNetworkManager::hasPendingMessage() {
    return !socketEmpty;
}

//--------------------
// PROTECTED METHODS
//--------------------

/**
 * Returns the structure with the server address in it.
 */
struct sockaddr_in ServerNetworkManager::getMyAddress(unsigned short port) {
    struct sockaddr_in myAddress;
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = INADDR_ANY;
    myAddress.sin_port = htons(port);
    bzero(myAddress.sin_zero, 8);

    return myAddress;
}

/**
 * Creates a new socket with the correct options for listening
 * @return int the sockfd of the socket created
 */
int ServerNetworkManager::createSocket() {
    // 1) First, we create the socket
    int sockfd = NetworkManager::createSocket();

    // 2) Then, we set the right options using setsockopt
    int yes = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(res < 0)
        throw std::runtime_error("Error while creating socket: setsockopt failed.");

    return sockfd;
}
