#include <iostream>
#include "packet.h"
#include "servernetwork.h"

using namespace std;

ServerNetwork::ServerNetwork()
{
    // Have the listener listen on the port
    listener.listen(defaultPort);

    // Add the listener to the selector
    selector.add(listener);
}

void ServerNetwork::receiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (threadsRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        IpPort address;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address.ip, address.port) == sf::Socket::Done)
            storePacket(packet, address);
    }
    cout << "ReceiveUdp finished.\n";
}

void ServerNetwork::receiveTcp()
{
    while (threadsRunning)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
                addClient();
            else
                testSockets();
        }
    }
}

bool ServerNetwork::arePackets()
{
    return !packets.empty();
}

PacketExtra& ServerNetwork::getPacket()
{
	return packets.front();
}

void ServerNetwork::popPacket()
{
	packets.pop_front();
}

void ServerNetwork::storePacket(sf::Packet& packet, ClientID sender)
{
	int type = -1;
	if (packet >> type && isValidType(type))
        packets.emplace_back(packet, sender);
}

void ServerNetwork::storePacket(sf::Packet& packet, const IpPort& address)
{
    storePacket(packet, clients.getIdFromAddress(address));
}

void ServerNetwork::sendToAll(sf::Packet& packet, ClientID exclude)
{
    clients.sendToAll(packet, exclude);
}

void ServerNetwork::sendToClient(sf::Packet& packet, ClientID clientID)
{
    clients.sendToClient(packet, clientID);
}

void ServerNetwork::addClient()
{
    // The listener is ready: there is a pending connection
    sf::TcpSocket* tcpSock = new sf::TcpSocket;
    //unique_ptr<sf::TcpSocket> tcpSock(new sf::TcpSocket);
    // TODO: Maybe use a shared_ptr?
    if (listener.accept(*tcpSock) == sf::Socket::Done)
    {
        // Add the new client to the selector
        selector.add(*tcpSock);
        // Add the new client to the map
        clients.addClient(tcpSock);
    }
    else
        delete tcpSock;
}

void ServerNetwork::removeClient(ClientID id)
{
    if (clients.validClientID(id))
    {
        auto clientToRemove = clients.getClientFromId(id);
        if (clientToRemove != nullptr)
        {
            selector.remove(*(clientToRemove->tcpSock));
            clients.removeClient(id);
        }
        else
            cerr << "Error: Failed to remove client because it doesn't exist?\n";
    }
}

void ServerNetwork::testSockets()
{
    // The listener socket is not ready, test all other sockets (the clients)
    for (auto& c: clients.getClientMap())
    {
        auto& tcpSock = *(c.second->tcpSock);
        if (selector.isReady(tcpSock))
        {
            ClientID senderId = c.second->id;
            // The client has sent some data, we can receive it
            sf::Packet packet;
            if (tcpSock.receive(packet) == sf::Socket::Done)
                storePacket(packet, senderId);
            else // the client has disconnected, so remove it
                removeClient(senderId);
        }
    }
}

bool ServerNetwork::isValidType(int type)
{
    return (type >= 0 && type < Packet::PacketTypes);
}
