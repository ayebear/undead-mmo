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

Client* ClientManager::getClientFromAddress(const IpPort& address)
{
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
