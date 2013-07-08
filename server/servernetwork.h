#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <map>
#include <string>
#include <deque>
#include "network.h"
#include "linkedqueue.h"
#include "clientmanager.h"
#include "miscnetwork.h"

class ServerNetwork: public Network
{
    public:
        ServerNetwork();

        // Overidden functions
        void receiveUdp();
        void receiveTcp();

        // Packet functions
        bool arePackets();
        PacketExtra& getPacket();
        void popPacket();
        void storePacket(sf::Packet&, ClientID);
        void storePacket(sf::Packet&, const IpPort&);

        // Clients
        void addClient();
        void removeClient(ClientID);

        // Other functions
        void sendToAll(sf::Packet&, ClientID exclude = -1);
        void sendToClient(sf::Packet&, ClientID);
        void testSockets();
        bool isValidType(int);

    private:
        std::deque<PacketExtra> packets; // Stores all received packets
        sf::TcpListener listener;
        sf::SocketSelector selector;
        ClientManager clients;
};

#endif
