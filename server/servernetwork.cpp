// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "packet.h"
#include "servernetwork.h"

using namespace std;

ServerNetwork::ServerNetwork(AccountDb& a, ClientManager& c, MasterEntityList& e, PacketExtra& u, PacketExtra& t):
    accounts(a),
    clients(c),
    entList(e),
    udpPacket(u),
    tcpPacket(t)
{
    udpSock.setBlocking(true); // Set the UDP socket to blocking mode
    udpSock.bind(serverPort); // Bind the UDP port
    listener.listen(serverPort); // Have the listener listen on the port
    selector.add(listener); // Add the listener to the selector
}

void ServerNetwork::receiveUdpPacket()
{
    IpPort address;
    // Will block on this line until a packet is received
    // It will loop until it receives a valid packet
    while ((udpSock.receive(udpPacket.data, address.ip, address.port) != sf::Socket::Done) ||
            !validatePacket(udpPacket, clients.getIdFromAddress(address)));
}

void ServerNetwork::receiveTcpPacket()
{
    receivedTcpPacket = false;
    while (!receivedTcpPacket)
    {
        // Make the selector wait for data on any socket
        //cout << "selector.wait()\n";
        if (selector.wait())
        {
            // Check if a new client is trying to connect
            if (selector.isReady(listener))
                addClient();
            else
                testSockets();
        }
    }
}

void ServerNetwork::sendToAllTcp(sf::Packet& packet, ClientID exclude, bool mustBeLoggedIn)
{
    sf::Lock clientLock(clients.getClientsMutex());
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
    sf::Lock clientLock(clients.getClientsMutex());
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
    if (c->isLoggedIn() || !mustBeLoggedIn)
        udpSock.send(packet, c->address.ip, clientPort); // TODO: Figure out a solution for UDP to not have to be port forwarded, and maybe if necessary have the port OS defined to prevent conflicts
        //udpSock.send(packet, c->address.ip, c->address.port);
}

void ServerNetwork::sendServerChatMessage(const string& msg, ClientID exclude)
{
    cout << msg << endl;
    sf::Packet packetToSend;
    packetToSend << Packet::ChatMessage << Packet::Chat::Server << msg;
    sendToAllTcp(packetToSend, exclude);
}

void ServerNetwork::addClient()
{
    //cout << "addClient()\n";
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
    //cout << "removeClient(" << id << ")\n";
    if (clients.validClientID(id))
    {
        Client* clientToRemove = clients.getClientFromId(id);
        if (clientToRemove != nullptr)
        {
            if (clientToRemove->isLoggedIn()) // Only do the on-logout stuff if they were logged in
                logOutClient(clientToRemove);
            else
                cout << "Removing client that is not logged in!\n";
            selector.remove(*(clientToRemove->tcpSock));
            clients.removeClient(id);
        }
    }
}

void ServerNetwork::logOutClient(Client* clientToRemove)
{
    // TODO: Move this stuff elsewhere (so that ServerNetwork does not need the account database and entity list!)
    // Save the player's position
    Entity* playerEnt = entList.find(clientToRemove->playerEid);
    if (playerEnt != nullptr)
    {
        sf::Vector2f pos = playerEnt->getPos();
        clientToRemove->pData->positionX = pos.x;
        clientToRemove->pData->positionY = pos.y;
    }
    entList.erase(clientToRemove->playerEid); // Remove the player's entity
    sendServerChatMessage(clientToRemove->username + " has logged out.", clientToRemove->id);
    accounts.saveAccount(*(clientToRemove->pData)); // Save their account data in the account database
    clientToRemove->logOut();
}

void ServerNetwork::testSockets()
{
    //cout << "testSockets()\n";
    // Check if any of the connected clients sent any data
    for (auto& c: clients.getClientMap())
    {
        auto& tcpSock = *(c.second->tcpSock);
        if (selector.isReady(tcpSock))
        {
            ClientID senderId = c.second->id;
            // Will block on this line until a packet is received
            //cout << "tcpSock.receive()\n";
            if (tcpSock.receive(tcpPacket.data) == sf::Socket::Done)
            {
                if (validatePacket(tcpPacket, senderId))
                {
                    receivedTcpPacket = true;
                    return;
                    // TODO: Figure out a better way to do this so that some clients are not prioritized over others
                }
            }
            else // The client has disconnected, so remove it
                removeClient(senderId);
        }
    }
}

bool ServerNetwork::validatePacket(PacketExtra& packet, ClientID senderId)
{
    packet.sender = senderId; // Store the client ID
    packet.extractType(); // Extract and store the type of the packet
    return packet.isValid();
}
