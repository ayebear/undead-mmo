// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory>
#include <SFML/Network.hpp>
#include "network.h"
#include "miscnetwork.h"
#include "playerdata.h"
#include "entity.h"

// This class contains all of the temporary things needed for when a client is logged in
class Client: sf::NonCopyable
{
    public:
        Client(ClientID, sf::TcpSocket* tcpSockPtr);
        ~Client();

        void tcpSend(sf::Packet&, bool mustBeLoggedIn = true);
        void logIn(EID);
        void logOut();
        bool isLoggedIn() const;

        ClientID id; // This will also be used for the key in the clients map
        IpPort address; // IP address and port
        EID playerEid; // The entity ID of the client's player entity
        PlayerData pData; // Stores the player data for the client
        std::unique_ptr<sf::TcpSocket> tcpSock; // TCP socket for the connection to the client

    private:
        bool loggedIn; // They could be connected but not logged in
        // TODO: We should enforce a timeout on the TCP connection if not logged in
};

#endif
