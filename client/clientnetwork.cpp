// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "clientnetwork.h"

using namespace std;

ClientNetwork::ClientNetwork()
{
    connected = false;
    serverAddress = sf::IpAddress::LocalHost;
    tcpSock.setBlocking(true);
}

ClientNetwork::~ClientNetwork()
{
    cout << "ClientNetwork class destructor called.\n";
    tcpSock.disconnect();
    cout << "TCP Socket was disconnected.\n";
}

void ClientNetwork::receiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (threadsRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            storePacket(packet);
    }
    cout << "ReceiveUdp finished.\n";
}

void ClientNetwork::receiveTcp()
{
    cout << "ReceiveTcp started.\n";
    while (threadsRunning && tcpSock.getLocalPort())
    {
        cout << "ReceiveTcp looping...\n";
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            storePacket(packet);
    }
    cout << "ReceiveTcp finished.\n";
}

bool ClientNetwork::arePackets(int type)
{
    return !packets[type].empty();
}

sf::Packet& ClientNetwork::getPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	return packets[type].front();
}

void ClientNetwork::popPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	packets[type].pop_front();
}

void ClientNetwork::storePacket(sf::Packet& packet)
{
	int type = -1;
	if (packet >> type && isValidType(type))
	{
        sf::Lock lock(packetMutexes[type]);
        packets[type].push_back(packet);
    }
}

// TODO: Eventually make specific functions for building different packet types but we can just use this for now
void ClientNetwork::sendPacket(sf::Packet& packet)
{
    tcpSock.send(packet);
}

// This initiates a TCP socket connection to a server
bool ClientNetwork::connectToServer(const sf::IpAddress& address)
{
    serverAddress = address;
    tcpSock.setBlocking(true);
    if (connected)
    {
        tcpSock.disconnect();
        connected = false;
    }
    sf::Socket::Status status = tcpSock.connect(serverAddress, defaultPort);
    tcpSock.setBlocking(false);
    connected = (status == sf::Socket::Done);
    if (connected)
        launchThreads();
    return connected;
}

// This will send a login request to the currently connected server
int ClientNetwork::login(const string& username, const string& password)
{
    sf::Packet loginPacket;
    loginPacket << Packet::LogIn << username << password;
    tcpSock.send(loginPacket);
    // Now it must wait for a packet back from the server...
    // Then process that packet and determine if the login was successful.
    // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.

    // Wait until you get a response from the server for your login request
    while (!arePackets(Packet::AuthStatus)); // TODO: Have a timeout and sleep inside of the loop

    int status = Packet::Auth::UnknownFailure;
    getPacket(Packet::AuthStatus) >> status;
    popPacket(Packet::AuthStatus);

    return status;
}

void ClientNetwork::sendChatMessage(const string& msg)
{
    if (!msg.empty() && msg.front() != '/')
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << msg;
        tcpSock.send(msgPacket);
    }
}

const string ClientNetwork::getStatusString()
{
    string status;
    if (!connected)
        status = "Currently not connected to any server.";
    else
    {
        status = "Currently connected to: ";
        status += serverAddress.toString();
    }
    return status;
}

bool ClientNetwork::validAddress(sf::IpAddress address)
{
    return (address == serverAddress);
}

bool ClientNetwork::isConnected()
{
    return connected;
}

bool ClientNetwork::isValidType(int type)
{
    return (type >= 0 && type < Packet::PacketTypes);
}
