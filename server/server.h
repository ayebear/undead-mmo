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

const int serverPort = 55001;

class Server
{
    public:
        Server();
        void Start();
    private:
        void PrintWelcomeMsg();
        void MainLoop();
        void SendToClients(sf::Packet& packet, int exclude = -1);

        // Stores the clients
        std::deque<sf::TcpSocket*> clients;

        // Create a socket to listen to new connections
        sf::TcpListener listener;

        // Create a selector
        sf::SocketSelector selector;
};

#endif // SERVER_H
