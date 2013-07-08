#include <iostream>
#include "clientmanager.h"

using namespace std;

ClientID ClientManager::idCounter = 0;

ClientManager::ClientManager()
{
    //ctor
}

ClientManager::~ClientManager()
{
    //dtor
}

bool ClientManager::validAddress(const IpPort& address)
{
    return false;
}

bool ClientManager::validClientID(ClientID)
{
    return false;
}

ClientID ClientManager::getIdFromAddress(const IpPort& address)
{
    return clientIDs[address];
}

Client& ClientManager::getClientFromId(ClientID id)
{
    return *clients[id];
}

ClientManager::ClientMap& ClientManager::getClientMap()
{
    return clients;
}

void ClientManager::addClient(sf::TcpSocket* tcpSock)
{
    // Generate an ID for the new client
    ClientID newID = getNewID();

    // Add the new client to the clients list
    clients[newID] = ClientPtr(new Client(newID, tcpSock));

    cout << "Client " << clients[newID]->address.ip << ":" << clients[newID]->address.port << " connected, here is the current list:\n";
    printClients();
}

void ClientManager::removeClient(ClientID id)//, sf::SocketSelector& selector)
{
    cout << "Client " << clients[id]->address.ip << ":" << clients[id]->address.port << " disconnected, here is the current list:\n";
    //selector.remove(*(clients[id]->tcpSock));
    clients.erase(id);
    printClients();
}

void ClientManager::sendToAll(sf::Packet& packet, ClientID exclude)
{
    for (auto& client: clients) // loop through the connected clients
    {
        if (client.first != exclude) // don't send the packet back to the client who sent it!
            client.second->tcpSock->send(packet); // send the packet to the other clients
    }
}

void ClientManager::sendToClient(sf::Packet& packet, ClientID clientID)
{
    clients[clientID]->tcpSock->send(packet);
}

void ClientManager::printClients()
{
    for (auto& c: clients)
        cout << "ID: " << c.second->id << ", Address: " << c.second->address.ip << ":" << c.second->address.port << endl;
}

ClientID ClientManager::getNewID()
{
    return idCounter++;
}
