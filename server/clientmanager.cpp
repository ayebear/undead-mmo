#include <iostream>
#include "clientmanager.h"

using namespace std;

ClientID ClientManager::idCounter = 0;
const ClientID ClientManager::invalidID = -1;

ClientManager::ClientManager()
{
}

bool ClientManager::validAddress(const IpPort& address)
{
    return (clientIDs.find(address) != clientIDs.end());
}

bool ClientManager::validClientID(ClientID id)
{
    return (clients.find(id) != clients.end());
}

ClientID ClientManager::getIdFromAddress(const IpPort& address)
{
    auto found = clientIDs.find(address);
    if (found != clientIDs.end())
        return found->second;
    else
        return invalidID;
}

Client* ClientManager::getClientFromId(ClientID id)
{
    auto found = clients.find(id);
    if (found != clients.end())
        return found->second.get();
    else
        return nullptr;
}

ClientManager::ClientMap& ClientManager::getClientMap()
{
    return clients;
}

void ClientManager::addClient(sf::TcpSocket* tcpSock)
{
    ClientID newID = getNewID(); // Generate an ID for the new client

    clients[newID] = ClientPtr(new Client(newID, tcpSock)); // Add the new client to the clients list
    //clients.emplace(newID, new Client(newID, tcpSock)); // Not supported with GCC 4.7.3?
    auto addr = clients[newID]->address; // Get the address of the new client
    clientIDs[addr] = newID; // Store the ID of the new client in the address map

    cout << "Client " << addr.ip << ":" << addr.port << " connected.\n";
    printClients();
}

void ClientManager::removeClient(ClientID id)//, sf::SocketSelector& selector)
{
    auto addr = clients[id]->address;
    cout << "Client " << addr.ip << ":" << addr.port << " disconnected.\n";
    clients.erase(id);
    clientIDs.erase(addr);
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

void ClientManager::sendToClient(sf::Packet& packet, const IpPort& address)
{
    ClientID id = getIdFromAddress(address);
    if (id != invalidID)
        clients[id]->tcpSock->send(packet);
}

void ClientManager::printClients()
{
    if (clients.empty())
        cout << "No clients are currently connected.\n";
    else
    {
        cout << "Currently connected clients:\n";
        for (auto& c: clients)
            cout << "ID: " << c.second->id << ", Address: " << c.second->address.ip << ":" << c.second->address.port << endl;
    }
}

ClientID ClientManager::getNewID()
{
    return idCounter++;
}
