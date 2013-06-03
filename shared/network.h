// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <atomic>
#include <SFML/Network.hpp>

/*
This is a base class that the server and client will have their own sub-classes of.

What this class does:
    Manages all of the socket handling
    Stores all of the received packets
    Handles sending packets and data

How it works and how it is used:
    See the sub classes for where the packets are actually stored.
    Other classes can basically run a while loop and continue getting packets until there are none left
    The other classes themselves will handle the information in the packets.

Advantages of having a class like this:
    Reduces dependency problems by designing it like a bus
    It automatically handles locks and threads, outside classes don't need to worry about it
    Server and client can share a lot of similar code
    Packets can be sent through TCP or UDP and it would all still work the same!
    Filters out bad packets and packets with unknown types
    Ensures all packets are from authorized senders
*/

class Network
{
    public:
        Network();
        virtual ~Network();
        void LaunchThreads();
        void StopThreads();
        void ReceiveUdp();

        virtual void ReceiveTcp() = 0;
        virtual void StorePacket(sf::Packet&) = 0;

        static const unsigned short defaultPort;

    protected:
        sf::UdpSocket udpSock;
        sf::Thread udpThread;
        sf::Thread tcpThread;
        std::atomic_bool threadsRunning;
};

#endif
