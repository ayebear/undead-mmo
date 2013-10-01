// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

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
    sf::Lock lock(clientsMutex);
    return (clientIDs.find(address) != clientIDs.end());
}

bool ClientManager::validClientID(ClientID id)
{
    sf::Lock lock(clientsMutex);
    return (clients.find(id) != clients.end());
}

ClientID ClientManager::getIdFromAddress(const IpPort& address)
{
    sf::Lock lock(clientsMutex);
    auto found = clientIDs.find(address);
    if (found != clientIDs.end())
        return found->second;
    else
        return invalidID;
}

Client* ClientManager::getClientFromAddress(const IpPort& address)
{
    sf::Lock lock(clientsMutex);
    auto foundId = clientIDs.find(address); // Try to get an ID from the address
    if (foundId != clientIDs.end())
    {
        auto foundPtr = clients.find(foundId->second); // Try to get the client pointer from the ID
        if (foundPtr != clients.end())
            return foundPtr->second.get();
    }
    return nullptr;
}

Client* ClientManager::getClientFromId(ClientID id)
{
    sf::Lock lock(clientsMutex);
    auto found = clients.find(id);
    if (found != clients.end())
        return found->second.get();
    else
        return nullptr;
}

// TODO: Get rid of the address map and this crap, and use some kind of multiple-key map or a multi-comparable class as the key
Client* ClientManager::getClientFromUsername(const string& username)
{
    sf::Lock lock(clientsMutex);
    for (auto& c: clients)
    {
        if (c.second->username == username)
            return c.second.get();
    }
    return nullptr;
}

ClientManager::ClientMap& ClientManager::getClientMap()
{
    return clients;
}

sf::Mutex& ClientManager::getClientsMutex()
{
    return clientsMutex;
}

void ClientManager::addClient(sf::TcpSocket* tcpSock)
{
    sf::Lock lock(clientsMutex);
    ClientID newID = getNewID(); // Generate an ID for the new client
    clients[newID].reset(new Client(newID, tcpSock)); // Add the new client to the clients list
    auto addr = clients[newID]->address; // Get the address of the new client
    clientIDs[addr] = newID; // Store the ID of the new client in the address map
    cout << "Client " << addr.ip << ":" << addr.port << " connected.\n";
    printClients();
}

void ClientManager::removeClient(ClientID id)//, sf::SocketSelector& selector)
{
    sf::Lock lock(clientsMutex);
    auto addr = clients[id]->address;
    cout << "Client " << addr.ip << ":" << addr.port << " disconnected.\n";
    clients.erase(id);
    clientIDs.erase(addr);
    printClients();
}

void ClientManager::printClients()
{
    sf::Lock lock(clientsMutex);
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
