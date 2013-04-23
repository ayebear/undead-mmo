// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
Implement threading
Also handle UDP packets
Send a message to all connected clients when a player connects or disconnects
Handle client timeouts and improper disconnects
Make a "protocol version" or something
Create a login system
Create a persistent account system
*/

#include "server.h"

using namespace std;

const ushort Server::port = 55001;

Server::Server()
{
}

void Server::Start()
{
    GetIPs();
    PrintWelcomeMsg();
    MainLoop();
}

void Server::GetIPs()
{
    severAddressLAN = sf::IpAddress::getLocalAddress();
    serverAddressWAN = sf::IpAddress::getPublicAddress();
}

void Server::PrintWelcomeMsg()
{
    cout << "ZombieSurvivalGame Server v0.1.1.2 Dev\n\n";
    cout << "The server's LAN IP Address is: " << severAddressLAN << endl;
    cout << "The server's WAN IP Address is: " << serverAddressWAN << endl;
}

void Server::SendToClients(sf::Packet& packet, int exclude)
{
    for (int i = 0; i != (int)clients.size(); ++i) // loop through the connected clients
    {
        if (i != exclude) // don't send the packet back to the client who sent it!
            clients[i]->send(packet); // send the packet to the other clients
    }
}

void Server::PrintClients()
{
    for (auto& c: clients)
        cout << c->getRemoteAddress() << endl;
}

void Server::AddClient()
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

void Server::RemoveClient(sf::TcpSocket& client, uint i)
{
    selector.remove(client);
    delete clients[i];
    clients.erase(clients.begin() + i);
}

void Server::TestSockets()
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
                ProcessPacket(packet, i);
            else // the client has disconnected, so remove it
            {
                cout << "Client " << client.getRemoteAddress() << " disconnected, here is the current list:\n";
                RemoveClient(client, i);
                PrintClients();
            }
        }
    }
}

void Server::ProcessPacket(sf::Packet& packet, uint exclude)
{
    int type = 1;
    packet >> type;
    switch (type)
    {
        case Packet::ChatMessage:{
            string msg;
            packet >> msg;
            cout << "Message: " << msg << endl;
            SendToClients(packet, exclude);
            break;}
        case Packet::PlayerUpdate:{
            float x, y;
            packet >> x >> y;
            // x and y need to be stored in the player object in the entity list
            // in the future this will be input and time instead of direct coordinates
            // also, when sending the data to the clients, it needs the entity ID with it of course
            SendToClients(packet, exclude);
            break;}
        default:
            cout << "Error: Unrecognized packet type? Type was: " << type << endl;
            break;
    }
}

void Server::MainLoop()
{
    // Have the listener listen on the port
    listener.listen(port);

    // Add the listener to the selector
    selector.add(listener);

    running = true;
    while (running)
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
