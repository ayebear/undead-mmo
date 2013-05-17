// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <list>
#include <SFML/Network.hpp>
#include "../shared/packet.h"

/*
This is a base class that the server and client will have their own sub-classes of.

What this class does:
    Manages all of the socket handling
    Stores all of the received packets
    Handles sending packets and data

How it works and how it is used:
    Stores an array (size is number of packet types) of lists of received packets.
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
/*

*/
/* TODO: Make this shared between the client and the server
Would need a read-only mode as well as a mode that pops packets/messages when reading them...
Would need a way to handle different senders for both TCP and UDP
    Could just store the sender's IP or something, the client must check anyway to make sure
        that the server sent the packet instead of another client.
    This would work with TCP as long as we did getRemoteAddress or whatever.
*/

class Network
{
    public:
        Network();
        virtual ~Network();
        void LaunchThreads();
        void StopThreads();
        bool ArePackets(int); // determines if there are any packets in the lists
        sf::Packet& GetPacket(int);
        void PopPacket(int);
        void StorePacket(sf::Packet&);
        void ReceiveUdp();
        virtual void ReceiveTcp() = 0;
        virtual const std::string GetStatusString() = 0;
        virtual bool ValidAddress(sf::IpAddress) = 0;

    protected:
        static const unsigned short defaultPort;
        sf::UdpSocket udpSock;

        sf::Thread udpThread;
        sf::Thread tcpThread;
        bool threadsRunning;

        // All received packets will be stored in here. Only valid packets sent from authorized senders will be stored.
        std::list<sf::Packet> packets[Packet::PacketTypes];
        sf::Mutex packetMutexes[Packet::PacketTypes];
        // If we need to store any other arrays this size, we should make a single array of structs with everything in them
};

#endif
