#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
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
        Client* getClientFromId(ClientID);
        ClientMap& getClientMap();

        void addClient(sf::TcpSocket*);
        void removeClient(ClientID);

        void sendToAll(sf::Packet&, ClientID exclude = -1);
        void sendToClient(sf::Packet&, ClientID);
        void sendToClient(sf::Packet&, const IpPort&);

        void printClients();

        static const ClientID invalidID;

    private:
        ClientID getNewID(); // Used to assign IDs to clients

        ClientMap clients; // Stores the clients, accessed by client ID
        ClientIDMap clientIDs; // Stores client IDs, accessed by IP address + port
        static ClientID idCounter; // Used by getNewID()
};

#endif
