// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <thread>
#include <SFML/Network.hpp>
#include "network.h"
#include "../shared/packet.h"
#include "../shared/other.h"

using namespace std;

Network::Network()
{
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
    status = tcpSock.connect(serverAddress, 55001);
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
    thread udpThread(&Network::ReceiveUdp, this);
    udpThread.detach();
    thread tcpThread(&Network::ReceiveTcp, this);
    tcpThread.detach();
}

void Network::ReceiveUdp()
{
    while (true)
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            ProcessPacket(packet);
    }
}

void Network::ReceiveTcp()
{
    // TODO: When the connection terminates, terminate the loop and disconnect
    while (true)
    {
        sf::Packet packet;
        // Will block on this line until a packet is received...
        if (tcpSock.receive(packet) == sf::Socket::Done)
            ProcessPacket(packet);
    }
}

// This works with both TCP or UDP, so we can change protocols for better robustness/performance
// This will need access to a lot of the stuff in the Game class...
// Currently needs access to: Chat, EntityList
void Network::ProcessPacket(sf::Packet& packet)
{
    int type;
    packet >> type;
    switch (type)
    {
        case Packet::ChatMessage:{
            string message;
            packet >> message;
            //theHud.chat.PrintMessage(message);
            break;}
        case Packet::EntityUpdate:{
            EID entID;
            packet >> entID;
            //entList.UpdateEntity(entID, packet)
            break;}
        default:
            break;
    }
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
