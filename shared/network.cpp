// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "network.h"
#include <iostream>
#include <SFML/Network.hpp>
#include "packet.h"

using namespace std;

// TODO: Figure out a better solution for UDP ports...
const unsigned short Network::serverPort = 1337;
const unsigned short Network::clientPort = 1338;

Network::Network() : udpThread(&Network::receiveUdp, this),
                     tcpThread(&Network::receiveTcp, this)
{
    udpSock.setBlocking(true);
    //udpSock.bind(defaultPort);
    tcpThreadRunning = false;
    udpThreadRunning = false;
}

Network::~Network()
{
    cout << "Network class destructor called.\n";
    udpSock.setBlocking(false);
    udpSock.unbind();
    cout << "UDP Socket was unbound.\n";
    stopThreads();
}

void Network::launchThreads()
{
    if (!udpThreadRunning)
    {
        udpThreadRunning = true;
        udpThread.launch();
    }
    if (!tcpThreadRunning)
    {
        tcpThreadRunning = true;
        tcpThread.launch();
    }
}

void Network::stopThreads()
{
    cout << "stopThreads(): Terminating threads now...\n";
    udpThread.terminate();
    cout << "UDP thread terminated.\n";
    tcpThread.terminate();
    cout << "TCP thread terminated.\n";
    udpThreadRunning = false;
    tcpThreadRunning = false;
}
