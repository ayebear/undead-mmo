#include "clientnetwork.h"

using namespace std;

ClientNetwork::ClientNetwork()
{
    connected = false;
    serverAddress = sf::IpAddress::LocalHost;
    tcpSock.setBlocking(true);
}

void ClientNetwork::ReceiveTcp()
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
    if (serverAddress == sf::IpAddress::None)
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
