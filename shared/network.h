// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <vector>
#include <list>
#include "../shared/packet.h"

// This class manages the sending and receiving of packets over the network for the client.
/*
This will store an array/vector (size is number of packet types) of lists/deques/vectors of messages/packets.
Other classes will be given a pointer/reference to specific packet types.
    For example, the chat class will only need the types of chat messages.
Those classes themselves will handle the information in the packets.
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
        bool ConnectToServer(const sf::IpAddress&);
        bool Login(const std::string&, const std::string&);
        void LaunchThreads();
        sf::Packet& GetPacket(int);
        void PopPacket(int);
        void StorePacket(sf::Packet&);
        void ReceiveUdp();
        void ReceiveTcp();
        void SendChatMessage(const std::string&);

    private:
        static const unsigned short defaultPort;
        sf::TcpSocket tcpSock;
        sf::UdpSocket udpSock;

        // Must be stored so that the UDP socket knows where to send packets to!
        sf::IpAddress serverAddress;

        std::list<sf::Packet> packets[Packet::PacketTypes];
        sf::Mutex packetMutex;

        bool connected;
};

#endif
