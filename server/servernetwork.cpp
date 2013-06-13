#include <iostream>
#include "servernetwork.h"

using namespace std;

Client::Client(sf::TcpSocket* tcpSockPtr)
    : loggedIn(false),
    id(idCounter++)
{
    tcpSock = tcpSockPtr;
    ip = tcpSock->getRemoteAddress();
    port = tcpSock->getRemotePort();
}

Client::~Client()
{
    if (tcpSock != nullptr)
        delete tcpSock;
}

ServerNetwork::ServerNetwork()
{
    // Have the listener listen on the port
    listener.listen(defaultPort);

    // Add the listener to the selector
    selector.add(listener);
}

void ServerNetwork::ReceiveUdp()
{
    cout << "ReceiveUdp started.\n";
    while (threadsRunning && udpSock.getLocalPort())
    {
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short port;
        // Will block on this line until a packet is received...
        if (udpSock.receive(packet, address, port) == sf::Socket::Done)
            StorePacket(packet, address);
    }
    cout << "ReceiveUdp finished.\n";
}

void ServerNetwork::ReceiveTcp()
{
    while (threadsRunning)
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

bool ServerNetwork::ArePackets()
{
    return !packets.empty();
}

PacketExtra& ServerNetwork::GetPacket()
{
	return packets.front();
}

void ServerNetwork::PopPacket()
{
	packets.pop_front();
}

void ServerNetwork::StorePacket(sf::Packet& packet, ClientID sender)
{
	int type = -1;
	if (packet >> type && IsValidType(type))
	{
        PacketExtra tmpPacket;
        tmpPacket.data = packet;
        tmpPacket.sender = sender;
        packets.push_back(tmpPacket);
	}
}

void ServerNetwork::StorePacket(sf::Packet& packet, sf::IpAddress& address)
{

}

const string ServerNetwork::GetStatusString()
{
    string status;
    for (auto& c: clients)
        status += c.second.tcpSock->getRemoteAddress().toString() + '\n';
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

void ServerNetwork::SendToAll(sf::Packet& packet, ClientID exclude)
{
    for (auto& client: clients) // loop through the connected clients
    {
        if (client.first != exclude) // don't send the packet back to the client who sent it!
            clients[client.first].tcpSock->send(packet); // send the packet to the other clients
    }
}

void ServerNetwork::SendToClient(sf::Packet& packet, ClientID clientID)
{
    clients[clientID].tcpSock->send(packet);
}

void ServerNetwork::AddClient()
{
    // The listener is ready: there is a pending connection
    sf::TcpSocket* client = new sf::TcpSocket;
    if (listener.accept(*client) == sf::Socket::Done)
    {
        // Add the new client to the clients list
        clients.emplace_back(client);

        // Add the new client to the selector
        selector.add(*client);

        cout << "Client " << client->getRemoteAddress() << " connected, here is the current list:\n";
        PrintClients();
    }
}

void ServerNetwork::RemoveClient(ClientID id)
{
    selector.remove(clients[id].tcpSock);
    clients.erase(clients.begin() + id);
}

void ServerNetwork::TestSockets()
{
    // The listener socket is not ready, test all other sockets (the clients)
    for (uint i = 0; i < clients.size(); ++i)
    {
        auto& client = *clients[i].tcpSock;
        if (selector.isReady(client))
        {
            // The client has sent some data, we can receive it
            sf::Packet packet;
            if (client.receive(packet) == sf::Socket::Done)
                StorePacket(packet, i); // TODO: Store sender also
            else // the client has disconnected, so remove it
            {
                cout << "Client " << client.getRemoteAddress() << " disconnected, here is the current list:\n";
                RemoveClient(client, i);
                PrintClients();
            }
        }
    }
}

bool ServerNetwork::IsValidType(int type)
{
    return (type >= 0 && type < Packet::PacketTypes);
}
