/* 
 * File:   servernetworkmanager.hpp
 * Author: neo
 *
 * Created on 10 février 2010, 12:46
 */

#ifndef _SERVERNETWORKMANAGER_HPP
#define	_SERVERNETWORKMANAGER_HPP

#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h> 

#include "NetworkManager.hpp"

#define SERVERPORT 1515
#define BACKLOG_SIZE 4

/**
 * The network communication class used on the server side.
 */
class ServerNetworkManager : public NetworkManager {
public:
    static ServerNetworkManager* getInstance();
    static void killInstance();

    /**
     * Initiates everything for the server to work properly
     */
    virtual void getReady();

    /**
     * Waits for another client to connect, accepts it and gives him a propre
     * communication socket.
     */
    virtual void acceptConnection();

    /**
     * Tells if the server is still accepting connections
     * @return true if the server isn't full, false otherwise
     */
    virtual bool isAcceptingConnections();

    /**
     * Asks the server to stop getting new connections.
     */
    virtual void stopAcceptingConnections();

    /**
     * This method checks whether there is at least one pending message waiting
     * in the socket.
     */
    virtual bool hasPendingMessage();
protected:
    virtual struct sockaddr_in getMyAddress(unsigned short port);
    virtual int createSocket();
private:
    ServerNetworkManager();
    virtual ~ServerNetworkManager();

    static ServerNetworkManager* instance_;

    bool ready;
    int listeningSocket;
    
    fd_set servicesSet;
};

#endif	/* _SERVERNETWORKMANAGER_HPP */

