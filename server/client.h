#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>
#include <SFML/Network.hpp>
#include "network.h"
#include "miscnetwork.h"
#include "playerdata.h"

class Client: sf::NonCopyable
{
    public:
        Client(ClientID, sf::TcpSocket* tcpSockPtr);
        ~Client();

        void tcpSend(sf::Packet&, bool mustBeLoggedIn = true);
        void logIn(const std::string&);
        void logOut();

        ClientID id; // This will also be used for the key in the clients map
        std::unique_ptr<sf::TcpSocket> tcpSock; // TCP socket for the connection to the client
        IpPort address; // IP address and port
        std::string username; // Client's username
        bool loggedIn; // They could be connected but not logged in
        std::unique_ptr<PlayerData> pData; // Stores the player data for the client
};

#endif
