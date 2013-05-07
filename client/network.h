// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <string>

// This class manages the sending and receiving of packets over the network for the client.
class Network
{
    public:
        Network();
        bool ConnectToServer(const sf::IpAddress&);
        bool Login(const std::string&, const std::string&);
        void LaunchThreads();
        void ReceiveUdp();
        void ReceiveTcp();
        void ProcessPacket(sf::Packet&);
        void SendChatMessage(const std::string&);

    private:
        sf::TcpSocket tcpSock;
        sf::UdpSocket udpSock;

        // Must be stored so that the UDP socket knows where to send packets to!
        sf::IpAddress serverAddress;

        bool connected;
};

#endif
