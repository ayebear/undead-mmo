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

void ClientNetwork::ReceiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (threadsRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            StorePacket(packet);
    }
    cout << "ReceiveUdp finished.\n";
}

void ClientNetwork::ReceiveTcp()
{
    cout << "ReceiveTcp started.\n";
    while (threadsRunning && tcpSock.getLocalPort())
    {
        cout << "ReceiveTcp looping...\n";
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            StorePacket(packet);
    }
    cout << "ReceiveTcp finished.\n";
}

bool ClientNetwork::ArePackets(int type)
{
    return !packets[type].empty();
}

sf::Packet& ClientNetwork::GetPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	return packets[type].front();
}

void ClientNetwork::PopPacket(int type)
{
	sf::Lock lock(packetMutexes[type]);
	packets[type].pop_front();
}

void ClientNetwork::StorePacket(sf::Packet& packet)
{
	int type = -1;
	if (packet >> type && IsValidType(type))
	{
        sf::Lock lock(packetMutexes[type]);
        packets[type].push_back(packet);
    }
}

// TODO: Eventually make specific functions for building different packet types but we can just use this for now
void ClientNetwork::SendPacket(sf::Packet& packet)
{
    tcpSock.send(packet);
}

// This initiates a TCP socket connection to a server
bool ClientNetwork::ConnectToServer(const sf::IpAddress& address)
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
        LaunchThreads();
    return connected;
}

// This will send a login request to the currently connected server
int ClientNetwork::Login(const string& username, const string& password)
{
    sf::Packet loginPacket;
    loginPacket << Packet::LogIn << username << password;
    tcpSock.send(loginPacket);
    // Now it must wait for a packet back from the server...
    // Then process that packet and determine if the login was successful.
    // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.

    // Wait until you get a response from the server for your login request
    while (!ArePackets(Packet::AuthStatus)); // TODO: Have a timeout and sleep inside of the loop

    int status = Packet::Auth::UnknownFailure;
    GetPacket(Packet::AuthStatus) >> status;
    PopPacket(Packet::AuthStatus);

    return status;
}

void ClientNetwork::SendChatMessage(const string& msg)
{
    if (!msg.empty() && msg.front() != '/')
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << msg;
        tcpSock.send(msgPacket);
    }
}

const string ClientNetwork::GetStatusString()
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

bool ClientNetwork::ValidAddress(sf::IpAddress address)
{
    return (address == serverAddress);
}

bool ClientNetwork::IsConnected()
{
    return connected;
}

bool ClientNetwork::IsValidType(int type)
{
    return (type >= 0 && type < Packet::PacketTypes);
}
