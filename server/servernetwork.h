#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <map>
#include <string>
#include "network.h"
#include "linkedqueue.h"
#include "clientmanager.h"
#include "miscnetwork.h"
#include "accountdb.h"

class ServerNetwork: public Network
{
    public:
        ServerNetwork(AccountDb&);

        // Overidden functions
        void receiveUdp();
        void receiveTcp();

        // Packet functions
        bool arePackets();
        PacketExtra& getPacket();
        void popPacket();

        // Send packets over the network
        // TCP
        void sendToAllTcp(sf::Packet&, ClientID exclude = -1, bool mustBeLoggedIn = true);
        void sendToClientTcp(sf::Packet&, ClientID, bool mustBeLoggedIn = true);
        void sendToClientTcp(sf::Packet&, const IpPort&, bool mustBeLoggedIn = true);
        // UDP
        void sendToAllUdp(sf::Packet&, ClientID exclude = -1, bool mustBeLoggedIn = true);
        void sendToClientUdp(sf::Packet&, ClientID, bool mustBeLoggedIn = true);
        void sendToClientUdp(sf::Packet&, const IpPort&, bool mustBeLoggedIn = true);
        void udpSend(Client*, sf::Packet&, bool mustBeLoggedIn = true);

        Client* getClientFromUsername(const std::string&);
        Client* getClientFromId(ClientID);

        void sendServerChatMessage(const std::string&, ClientID exclude = -1);

    private:
        void storePacket(sf::Packet&, ClientID);
        void storePacket(sf::Packet&, const IpPort&);

        void addClient();
        void removeClient(ClientID);
        void testSockets();
        bool isValidType(int);

        LinkedQueue<PacketExtra> packets; // Stores all received packets
        sf::TcpListener listener;
        sf::SocketSelector selector;
        ClientManager clients;
        AccountDb& accounts;
};

#endif
