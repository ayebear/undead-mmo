#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <deque>
#include "../shared/network.h"

struct PacketExtra
{
    sf::Packet packet;
    int sender;
};

class ServerNetwork: public Network
{
    public:
        ServerNetwork();
        void ReceiveTcp();
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);

        void SendToAll(sf::Packet&, int exclude = -1);
        void SendToClient(sf::Packet&, int);
        void PrintClients();
        void AddClient();
        void RemoveClient(sf::TcpSocket&, uint);
        void TestSockets();
        void ProcessPacket(sf::Packet&, uint);
    private:
        std::deque<sf::TcpSocket*> clients; // Stores the clients
        sf::TcpListener listener;
        sf::SocketSelector selector;
};

#endif
