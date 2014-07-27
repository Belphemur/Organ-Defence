/* 
 * File:   clientnetworkmanager.hpp
 * Author: neo
 *
 * Created on 10 février 2010, 12:46
 */

#ifndef _CLIENTNETWORKMANAGER_HPP
#define	_CLIENTNETWORKMANAGER_HPP

#include <string>
#include <netdb.h>

#include "NetworkManager.hpp"

class ClientNetworkManager : public NetworkManager {
public:
    static ClientNetworkManager* getInstance();
	static void killInstance();

    /**
     * Connects to the server whose name is given in parameter.
     * @param hostname The server name.
     * @param port The port to connect to
     */
    virtual void connectTo(std::string hostname);
protected:
    virtual struct sockaddr_in getServerAddress(std::string hostname, unsigned short port);
private:
    ClientNetworkManager();
    virtual ~ClientNetworkManager();

    static ClientNetworkManager* instance_;
    bool connected;
};

#endif	/* _CLIENTNETWORKMANAGER_HPP */

