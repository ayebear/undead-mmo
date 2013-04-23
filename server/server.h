// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <SFML/Network.hpp>
#include "../shared/packet.h"

class Server
{
    public:
        Server();
        void Start();
    private:
        void GetIPs();
        void PrintWelcomeMsg();
        void MainLoop();
        void SendToClients(sf::Packet& packet, int exclude = -1);
        void PrintClients();
        void AddClient();
        void RemoveClient(sf::TcpSocket&, uint);
        void TestSockets();
        void ProcessPacket(sf::Packet&, uint);

        static const ushort port;

        std::deque<sf::TcpSocket*> clients; // Stores the clients
        sf::TcpListener listener;
        sf::SocketSelector selector;
        sf::IpAddress severAddressLAN;
        sf::IpAddress serverAddressWAN;
        bool running;
};

#endif // SERVER_H
