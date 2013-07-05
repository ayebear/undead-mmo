// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <atomic>
#include <SFML/Network.hpp>

/*
This is a base class that the server and client will have their own sub-classes of.
This class just handles the threading for receiving UDP/TCP packets.
*/
class Network
{
    public:
        Network();
        virtual ~Network();

        void launchThreads();
        void stopThreads();

        virtual void receiveUdp() = 0;
        virtual void receiveTcp() = 0;

        static const unsigned short defaultPort;

    protected:
        sf::UdpSocket udpSock;
        sf::Thread udpThread;
        sf::Thread tcpThread;
        std::atomic_bool threadsRunning;
};

#endif
