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
    connected = false;
    threadsRunning = false;
    cout << "ReceiveUdp finished.\n";
}

void ClientNetwork::receiveTcp()
{
    cout << "ReceiveTcp started.\n";
    while (threadsRunning && tcpSock.getLocalPort())
    {
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            storePacket(packet);
    }
    connected = false;
    threadsRunning = false;
    cout << "ReceiveTcp finished.\n";
}

bool ClientNetwork::arePackets(int type)
{
    return !packets[type].empty();
}

sf::Packet& ClientNetwork::getPacket(int type)
{
	return packets[type].front();
}

void ClientNetwork::popPacket(int type)
{
	packets[type].pop_front();
}

void ClientNetwork::clearPackets(int type)
{
    packets[type].clear();
}

void ClientNetwork::storePacket(sf::Packet& packet)
{
	int type = -1;
	if (packet >> type)
	{
        if (isValidType(type))
            packets[type].push_back(packet);
    }
}

// TODO: Eventually make specific functions for building different packet types but we can just use this for now
void ClientNetwork::sendPacketTcp(sf::Packet& packet)
{
    if (connected)
        tcpSock.send(packet);
}

void ClientNetwork::sendPacketUdp(sf::Packet& packet)
{
    if (connected)
        udpSock.send(packet, serverAddress, defaultPort);
}

void ClientNetwork::sendChatMessage(const string& msg)
{
    if (!msg.empty())
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << Packet::Chat::Public << msg;
        sendPacketTcp(msgPacket);
    }
}

void ClientNetwork::sendChatMessage(const string& msg, const string& username)
{
    if (!msg.empty() && !username.empty())
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << Packet::Chat::Private << username << msg;
        sendPacketTcp(msgPacket);
    }
}

void ClientNetwork::setServerAddress(const sf::IpAddress& address)
{
    serverAddress = address;
}

int ClientNetwork::logIn(const sf::IpAddress& address, const string& username, const string& password)
{
    serverAddress = address;
    return logIn(username, password);
}

// This will send a log in request to the currently connected server
int ClientNetwork::logIn(const string& username, const string& password)
{
    currentUsername = username;
    int status = Packet::Login::ErrorConnecting;
    if (connected || connectToServer())
    {
        status = Packet::Login::UnknownFailure;

        clearPackets(Packet::LoginStatus);

        sf::Packet loginPacket;
        loginPacket << Packet::LogIn << username << password;
        tcpSock.send(loginPacket);

        if (connected)
            cout << "Currently connected to server.\n";
        else
            cout << "Currently NOT connected to server.\n";

        cout << "Sent log in packet. Now waiting for a response...\n";

        // Wait until you get a response from the server for your log in request
        // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.
        //      This would be nice to see a logging in thing of some sort.
        int timeout = 10;
        sf::Clock loginTimer;
        while (!arePackets(Packet::LoginStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
            sf::sleep(sf::milliseconds(10));

        if (arePackets(Packet::LoginStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            getPacket(Packet::LoginStatus) >> status;
            popPacket(Packet::LoginStatus);
        }
        else
        {
            status = Packet::Login::Timeout;
            cout << "Log in timed out.\n";
        }
    }
    cout << "Login status: " << status << endl;
    return status;
}

void ClientNetwork::logOut()
{
    cout << "Logged out from server.\n";
    sf::Packet logOutPacket;
    logOutPacket << Packet::LogOut;
    tcpSock.send(logOutPacket);
}

const string& ClientNetwork::getUsername()
{
    return currentUsername;
}

// This initiates a TCP socket connection to a server
bool ClientNetwork::connectToServer()
{
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
