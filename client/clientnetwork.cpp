// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "clientnetwork.h"
#include <iostream>

using namespace std;

ClientNetwork::ClientNetwork()
{
    // Bind the UDP port
    udpSock.bind(clientPort);

    connected = false;
    serverAddress.ip = sf::IpAddress::LocalHost;
    serverAddress.port = serverPort;
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
    while (udpThreadRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        IpPort address;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address.ip, address.port) == sf::Socket::Done)
        {
            // Only accept packets from the server!
            if (address == serverAddress)
                storePacket(packet);
        }
    }
    connected = false;
    udpThreadRunning = false;
    cout << "ReceiveUdp finished.\n";
}

void ClientNetwork::receiveTcp()
{
    cout << "ReceiveTcp started.\n";
    while (tcpThreadRunning && tcpSock.getLocalPort())
    {
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            storePacket(packet);
    }
    connected = false;
    tcpThreadRunning = false;
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

void ClientNetwork::clearAllPackets()
{
    // Clear all packets
    for (auto& p: packets)
        p.clear();
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
        udpSock.send(packet, serverAddress.ip, serverAddress.port);
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
    serverAddress.ip = address;
}

// TODO: Use an ip/port combo instead so that a single IP can host multiple servers
int ClientNetwork::logIn(const sf::IpAddress& address, const string& username, const string& password)
{
    serverAddress.ip = address;
    return logIn(username, password);
}

// This will send a log in request to the currently connected server
int ClientNetwork::logIn(const string& username, const string& password)
{
    clearAllPackets();
    currentUsername = username;
    int status = Packet::LogInCode::ErrorConnecting;
    if (connected || connectToServer())
    {
        status = Packet::LogInCode::UnknownFailure;

        clearPackets(Packet::LogInStatus);

        sf::Packet loginPacket;
        loginPacket << Packet::LogIn << Packet::ProtocolVersion << username << password;
        tcpSock.send(loginPacket);

        cout << "Sent log in packet. Now waiting for a response...\n";

        // Wait until you get a response from the server for your log in request
        // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.
        //      This would be nice to see a logging in thing of some sort.
        int timeout = 10;
        sf::Clock loginTimer;
        while (!arePackets(Packet::LogInStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
            sf::sleep(sf::milliseconds(10));

        if (arePackets(Packet::LogInStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            getPacket(Packet::LogInStatus) >> status;
            popPacket(Packet::LogInStatus);
        }
        else
        {
            status = Packet::LogInCode::Timeout;
            cout << "Log in timed out.\n";
        }
    }
    cout << "Login status: " << status << endl;
    // Prevent possibility of client receiving invalid status code from server
    if (status < 1 || status > Packet::LogInCode::UnknownFailure)
        status = Packet::LogInCode::UnknownFailure;
    return status;
}

int ClientNetwork::createAccount(const sf::IpAddress& address, const string& username, const string& password)
{
    serverAddress.ip = address;
    return createAccount(username, password);
}

// This will send a create account request to the currently connected server
int ClientNetwork::createAccount(const string& username, const string& password)
{
    clearAllPackets();
    int status = Packet::CreateAccountCode::ErrorConnecting;
    if (connected || connectToServer())
    {
        status = Packet::CreateAccountCode::UnknownFailure;

        clearPackets(Packet::CreateAccountStatus);

        sf::Packet createAccountPacket;
        createAccountPacket << Packet::CreateAccount << Packet::ProtocolVersion << username << password;
        tcpSock.send(createAccountPacket);

        cout << "Sent create account packet. Now waiting for a response...\n";

        // Wait until you get a response from the server for your log in request
        // Maybe the Game class can sort of handle this... So we can just use the threaded receive loops.
        //      This would be nice to see a logging in thing of some sort.
        int timeout = 10;
        sf::Clock createAccountTimer;
        while (!arePackets(Packet::CreateAccountStatus) && createAccountTimer.getElapsedTime().asSeconds() < timeout)
            sf::sleep(sf::milliseconds(10));

        if (arePackets(Packet::CreateAccountStatus) && createAccountTimer.getElapsedTime().asSeconds() < timeout)
        {
            getPacket(Packet::CreateAccountStatus) >> status;
            popPacket(Packet::CreateAccountStatus);
        }
        else
        {
            status = Packet::CreateAccountCode::Timeout;
            cout << "Create account timed out.\n";
        }
    }
    cout << "Create account status: " << status << endl;
    // Prevent possibility of client receiving invalid status code from server
    if (status < 1 || status > Packet::CreateAccountCode::UnknownFailure)
        status = Packet::CreateAccountCode::UnknownFailure;
    return status;
}

void ClientNetwork::logOut()
{
    /*sf::Packet logOutPacket;
    logOutPacket << Packet::LogOut;
    tcpSock.send(logOutPacket);*/
    tcpSock.disconnect();
    tcpThreadRunning = false;
    connected = false;
    clearAllPackets();
    cout << "Logged out from server.\n";
}

const string& ClientNetwork::getUsername()
{
    return currentUsername;
}

// This initiates a TCP socket connection to a server
bool ClientNetwork::connectToServer()
{
    if (connected)
    {
        tcpSock.disconnect();
        connected = false;
    }
    sf::Socket::Status status = tcpSock.connect(serverAddress.ip, serverAddress.port);
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
        status += serverAddress.ip.toString();
    }
    return status;
}

bool ClientNetwork::validAddress(sf::IpAddress address)
{
    return (address == serverAddress.ip);
}

bool ClientNetwork::isConnected()
{
    return connected;
}

bool ClientNetwork::isValidType(int type)
{
    return (type >= 0 && type < Packet::PacketTypes);
}
