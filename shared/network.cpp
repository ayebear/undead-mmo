// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <SFML/Network.hpp>
//#include <SFML/System.hpp>
#include "network.h"
#include "../shared/packet.h"
#include "../shared/other.h"

using namespace std;

const unsigned short Network::defaultPort = 55001;

Network::Network()
{
    connected = false;

    serverAddress = "ayebear.com";

    // The sockets should be blocking so they are dealt with as soon as data is received
    tcpSock.setBlocking(true);
    udpSock.setBlocking(true);
}

// This initiates a TCP socket connection to a server
bool Network::ConnectToServer(const sf::IpAddress& address)
{
    serverAddress = address;
    sf::Socket::Status status;
    tcpSock.setBlocking(true);
    status = tcpSock.connect(serverAddress, defaultPort);
    tcpSock.setBlocking(false);
    return (status == sf::Socket::Done);
}

// This will send a login request to the currently connected server
bool Network::Login(const string& username, const string& password)
{
    sf::Packet loginPacket;
    loginPacket << Packet::LogIn << username << password;
    tcpSock.send(loginPacket);
    // Now it must wait for a packet back from the server...
    // Then process that packet and determine if the login was successful.
    // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.
    return false;
}

void Network::LaunchThreads()
{
    sf::Thread udpThread(&Network::ReceiveUdp, this);
    udpThread.launch();
    sf::Thread tcpThread(&Network::ReceiveTcp, this);
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

void Network::ReceiveTcp()
{
    // TODO: When the connection terminates, terminate the loop and disconnect
    while (tcpSock.getLocalPort())
    {
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            StorePacket(packet);
    }
}

sf::Packet& Network::GetPacket(int type)
{
	sf::Lock lock(packetMutex);
	return packets[type].front();
}

void Network::PopPacket(int type)
{
	sf::Lock lock(packetMutex);
	packets[type].pop_front();
}

void Network::StorePacket(sf::Packet& packet)
{
	int type;
	packet >> type;
	sf::Lock lock(packetMutex);
	packets[type].push_back(packet);
}

void Network::SendChatMessage(const string& msg)
{
    if (!msg.empty() && msg.front() != '/')
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << msg;
        tcpSock.send(msgPacket);
    }
}
