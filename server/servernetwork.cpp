#include <iostream>
#include "servernetwork.h"

using namespace std;

ServerNetwork::ServerNetwork()
{
    // Have the listener listen on the port
    listener.listen(defaultPort);

    // Add the listener to the selector
    selector.add(listener);
}

void ServerNetwork::ReceiveTcp()
{
    while (true)
    {
        // Make the selector wait for data on any socket
        if (selector.wait())
        {
            // Test the listener
            if (selector.isReady(listener))
                AddClient();
            else
                TestSockets();
        }
    }
}

const string ServerNetwork::GetStatusString()
{
    string status;
    for (auto& c: clients)
        status += c->getRemoteAddress().toString() + '\n';
    return status;
}

void ServerNetwork::PrintClients()
{
    cout << GetStatusString();
}

bool ServerNetwork::ValidAddress(sf::IpAddress address)
{
    string clientsStr = GetStatusString();
    return (clientsStr.find(address.toString()) != string::npos);
}

void ServerNetwork::SendToAll(sf::Packet& packet, int exclude)
{
    for (int i = 0; i != (int)clients.size(); ++i) // loop through the connected clients
    {
        if (i != exclude) // don't send the packet back to the client who sent it!
            clients[i]->send(packet); // send the packet to the other clients
    }
}

void ServerNetwork::SendToClient(sf::Packet& packet, int clientID)
{
    clients[clientID]->send(packet);
}

void ServerNetwork::AddClient()
{
    // The listener is ready: there is a pending connection
    sf::TcpSocket* client = new sf::TcpSocket;
    if (listener.accept(*client) == sf::Socket::Done)
    {
        // Add the new client to the clients list
        clients.push_back(client);

        // Add the new client to the selector
        selector.add(*client);

        cout << "Client " << client->getRemoteAddress() << " connected, here is the current list:\n";
        PrintClients();
    }
}

void ServerNetwork::RemoveClient(sf::TcpSocket& client, uint i)
{
    selector.remove(client);
    delete clients[i];
    clients.erase(clients.begin() + i);
}

void ServerNetwork::TestSockets()
{
    // The listener socket is not ready, test all other sockets (the clients)
    for (uint i = 0; i < clients.size(); ++i)
    {
        auto& client = *clients[i];
        if (selector.isReady(client))
        {
            // The client has sent some data, we can receive it
            sf::Packet packet;
            if (client.receive(packet) == sf::Socket::Done)
                StorePacket(packet); // TODO: Store sender also
            else // the client has disconnected, so remove it
            {
                cout << "Client " << client.getRemoteAddress() << " disconnected, here is the current list:\n";
                RemoveClient(client, i);
                PrintClients();
            }
        }
    }
}
