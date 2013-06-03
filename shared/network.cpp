// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include <SFML/Network.hpp>
#include "network.h"
#include "../shared/packet.h"
#include "../shared/other.h"

using namespace std;

const unsigned short Network::defaultPort = 55001;

Network::Network() : udpThread(&Network::ReceiveUdp, this),
                     tcpThread(&Network::ReceiveTcp, this)
{
    udpSock.setBlocking(true);
    udpSock.bind(defaultPort);
    threadsRunning = false;
}

Network::~Network()
{
    cout << "Network class destructor called.\n";
    udpSock.setBlocking(false);
    udpSock.unbind();
    cout << "UDP Socket was unbound.\n";
    StopThreads();
}

void Network::LaunchThreads()
{
    if (!threadsRunning)
    {
        udpThread.launch();
        tcpThread.launch();
        threadsRunning = true;
    }
}

void Network::StopThreads()
{
    if (threadsRunning)
    {
        cout << "StopThreads(): Terminating threads now...\n";
        udpThread.terminate();
        tcpThread.terminate();
        threadsRunning = false;
    }
    else
        cout << "StopThreads(): No threads are running.\n";
}

void Network::ReceiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (threadsRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            StorePacket(packet);
    }
    cout << "ReceiveUdp finished.\n";
}
