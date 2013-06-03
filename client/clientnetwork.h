// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <string>
#include <list>
#include "../shared/network.h"
#include "../shared/packet.h"

/*
This class handles all of the client side networking.
It organizes received packets by their type into different lists inside of an array.
This way things can more easily read specific packets received instead of everything filtering through all of them.
*/
class ClientNetwork: public Network
{
    public:
        ClientNetwork();
        ~ClientNetwork();

        // Overridden functions
        void ReceiveTcp();
        void StorePacket(sf::Packet&);

        // Packet functions
        bool ArePackets(int); // determines if there are any packets in the lists
        sf::Packet& GetPacket(int);
        void PopPacket(int);

        // Other
        bool ConnectToServer(const sf::IpAddress&);
        int Login(const std::string&, const std::string&);
        void SendChatMessage(const std::string&);
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);
        bool IsConnected();
        bool IsValidType(int);
    private:
        // Client only needs a single TCP socket because it is only communicating with the server
        sf::TcpSocket tcpSock;
        sf::IpAddress serverAddress;
        bool connected;

        // All received packets will be stored in here. Only valid packets sent from the server will be stored.
        std::list<sf::Packet> packets[Packet::PacketTypes]; // TODO: Write a custom thread-safe linked list class instead
        sf::Mutex packetMutexes[Packet::PacketTypes];
};

#endif
