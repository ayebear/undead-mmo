#include <iostream>
#include "packet.h"
#include "servernetwork.h"

using namespace std;

ServerNetwork::ServerNetwork()
{
    // Bind the UDP port
    udpSock.bind(serverPort);

    // Have the listener listen on the port
    listener.listen(serverPort);

    // Add the listener to the selector
    selector.add(listener);
}

void ServerNetwork::receiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (udpThreadRunning && udpSock.getLocalPort())
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
    cout << "ReceiveTcp started.\n";
    while (tcpThreadRunning)
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
    cout << "ReceiveTcp finished.\n";
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
    if (sender != ClientManager::invalidID)
    {
        int type = -1;
        if (packet >> type)
        {
            //cout << "storePacket(): type = " << type << ", sender = " << sender << endl;
            if (isValidType(type))
                packets.push_back(PacketExtra(packet, sender, type)); // TODO: Write an emplace_back function or maybe use move semantics
            else
                cerr << "storePacket(): Error: Packet type " << type << " is invalid!\n";
        }
	}
}

void ServerNetwork::storePacket(sf::Packet& packet, const IpPort& address)
{
    storePacket(packet, clients.getIdFromAddress(address));
}

void ServerNetwork::sendToAllTcp(sf::Packet& packet, ClientID exclude, bool mustBeLoggedIn)
{
    sf::Lock lock(clients.getClientsMutex());
    for (auto& client: clients.getClientMap()) // Loop through the connected clients
    {
        // Only send the packet if they are not excluded (which is normally because that client sent the packet)
        if (client.first != exclude)
            client.second->tcpSend(packet, mustBeLoggedIn); // Send the packet
    }
}

void ServerNetwork::sendToClientTcp(sf::Packet& packet, ClientID clientID, bool mustBeLoggedIn)
{
    Client* c = clients.getClientFromId(clientID);
    if (c != nullptr)
        c->tcpSend(packet, mustBeLoggedIn);
}

void ServerNetwork::sendToClientTcp(sf::Packet& packet, const IpPort& address, bool mustBeLoggedIn)
{
    Client* c = clients.getClientFromAddress(address);
    if (c != nullptr)
        c->tcpSend(packet, mustBeLoggedIn);
}

void ServerNetwork::sendToAllUdp(sf::Packet& packet, ClientID exclude, bool mustBeLoggedIn)
{
    sf::Lock lock(clients.getClientsMutex());
    for (auto& client: clients.getClientMap()) // Loop through the connected clients
    {
        // Only send the packet if they are not excluded (which is normally because that client sent the packet)
        if (client.first != exclude)
            udpSend(client.second.get(), packet, mustBeLoggedIn); // Send the packet
    }
}

void ServerNetwork::sendToClientUdp(sf::Packet& packet, ClientID clientID, bool mustBeLoggedIn)
{
    Client* c = clients.getClientFromId(clientID);
    if (c != nullptr)
        udpSend(c, packet, mustBeLoggedIn);
}

void ServerNetwork::sendToClientUdp(sf::Packet& packet, const IpPort& address, bool mustBeLoggedIn)
{
    Client* c = clients.getClientFromAddress(address);
    if (c != nullptr)
        udpSend(c, packet, mustBeLoggedIn);
}

void ServerNetwork::udpSend(Client* c, sf::Packet& packet, bool mustBeLoggedIn)
{
    if (c->loggedIn || !mustBeLoggedIn)
        udpSock.send(packet, c->address.ip, clientPort); // TODO: Figure out a solution for UDP to not have to be port forwarded, and maybe if necessary have the port OS defined to prevent conflicts
        //udpSock.send(packet, c->address.ip, c->address.port);
}

// TODO: Get rid of the address map and this crap, and use some kind of multiple-key map or a multi-comparable class as the key
Client* ServerNetwork::getClientFromUsername(const string& username)
{
    sf::Lock lock(clients.getClientsMutex());
    for (auto& c: clients.getClientMap())
    {
        if (c.second->username == username)
            return c.second.get();
    }
    return nullptr;
}

Client* ServerNetwork::getClientFromId(ClientID id)
{
    return clients.getClientFromId(id);
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
    return (type >= 0 && type < Packet::TotalPacketTypes);
}
