// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include <SFML/Network.hpp>
#include "network.h"
#include "../shared/packet.h"

using namespace std;

const unsigned short Network::defaultPort = 1337;

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
