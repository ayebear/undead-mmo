// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "client.h"

class ClientManager
{
    public:
        typedef std::unique_ptr<Client> ClientPtr;
        typedef std::map<ClientID, ClientPtr> ClientMap;
        typedef std::map<IpPort, ClientID> ClientIDMap;

        ClientManager();

        bool validAddress(const IpPort&);
        bool validClientID(ClientID);
        ClientID getIdFromAddress(const IpPort&);
        Client* getClientFromAddress(const IpPort&);
        Client* getClientFromId(ClientID);
        Client* getClientFromUsername(const std::string&);
        ClientMap& getClientMap();
        sf::Mutex& getClientsMutex();

        void addClient(sf::TcpSocket*);
        void removeClient(ClientID);

        void printClients();

        static const ClientID invalidID;

    private:
        ClientID getNewID(); // Used to assign IDs to clients

        // TODO: Make this fully thread safe by having locks on the client pointers
        // Also actually use these locks outside of this class for things that access the clients
        sf::Mutex clientsMutex; // Lock for the clients map
        ClientMap clients; // Stores the clients, accessed by client ID
        ClientIDMap clientIDs; // Stores client IDs, accessed by IP address + port
        static ClientID idCounter; // Used by getNewID()
};

#endif
