#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include "../shared/network.h"

// TODO: Move most of the server code into here, the code that manages sockets and stuff

class ServerNetwork: public Network
{
    public:
        ServerNetwork();
        void ReceiveTcp();
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);

        void SendToClients(sf::Packet& packet, int exclude = -1);
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
