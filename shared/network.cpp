// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

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
}

// This is flawed logic - the thread objects are destroyed after this function finishes. So fix this by creating the thread objects outside of the class...
void Network::LaunchThreads()
{
    udpThread.launch();
    tcpThread.launch();
}

void Network::ReceiveUdp()
{
    while (udpSock.getLocalPort())
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            StorePacket(packet);
    }
}

bool Network::ArePackets(int type)
{
    return !packets[type].empty();
}

sf::Packet& Network::GetPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	return packets[type].front();
}

void Network::PopPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	packets[type].pop_front();
}

void Network::StorePacket(sf::Packet& packet)
{
	int type;
	packet >> type;
	if (type >= 0 && type < Packet::PacketTypes)
	{
        sf::Lock lock(packetMutexes[type]);
        packets[type].push_back(packet);
    }
}
