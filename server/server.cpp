// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
Send a message to all connected clients when a player connects or disconnects
Handle client timeouts and improper disconnects
See the tickets on sourceforge for more.
*/

#include "server.h"

using namespace std;

const float Server::desiredFrameTime = 1.0 / 120.0;

Server::Server()
{
}

void Server::Start()
{
    PrintWelcomeMsg();
    MainLoop();
}

void Server::PrintWelcomeMsg()
{
    cout << "ZombieSurvivalGame Server v0.2.1.1 Dev\n\n";
    cout << "The server's LAN IP Address is: " << sf::IpAddress::getLocalAddress() << endl;
    cout << "The server's WAN IP Address is: " << sf::IpAddress::getPublicAddress() << endl;
}

void Server::MainLoop()
{
    netManager.LaunchThreads();
    running = true;
    while (running)
    {
        // TODO: Put these in separate threads, so that all packets can be processed right away,
        // and updates happen at the desired frame rate.

        // Thread for ProcessAllPackets();
        // Thread for Update();

        // Do stuff with all of the received packets
        ProcessAllPackets();

        // Update the current game state, also send some of this info to the clients
        Update();

        // Calculate elapsed time
        elapsedTime = clock.restart().asSeconds();

        // Sleep some if everything is caught up (we will only want to do this with the update thread)
        //sf::sleep(sf::milliseconds(desiredFrameTime - elapsedTime));
    }
}

void Server::ProcessAllPackets()
{
    while (clock.getElapsedTime() < desiredFrameTime)
    {
        while (netManager.ArePackets())
            ProcessPacket(netManager.GetPacket());
    }
}

void Server::Update()
{
    // TODO: Iterate through the entity grid instead
    entList.Update(elapsedTime);
}

void Server::ProcessPacket(PacketExtra& packet)
{
    int type = 1;
    packet.data >> type;
    switch (type)
    {
        case Packet::ChatMessage:
            ProcessChatMessage(packet);
            break;
        case Packet::LogIn:
            ProcessLogIn(packet);
            break;
        default:
            cout << "Error: Unknown received packet type.\n";
            break;
    }
}

void Server::ProcessChatMessage(PacketExtra& packet)
{
    string msg;
    packet >> msg;
    cout << "Message from " << packet.sender << ": " << msg << endl;
    netManager.SendToAll(packet.data, packet.sender);
}

void Server::ProcessLogIn(PacketExtra& packet)
{
    string username, password;
}

/*
void Server::ProcessPacket(sf::Packet& packet, uint exclude)
{
    int type = 1;
    packet >> type;
    switch (type)
    {
        case Packet::ChatMessage:{

            break;}
        case Packet::EntityUpdate:{
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
*/
