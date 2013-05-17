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
    cout << "ZombieSurvivalGame Server v0.2.0.2 Dev\n\n";
    cout << "The server's LAN IP Address is: " << sf::IpAddress::getLocalAddress() << endl;
    cout << "The server's WAN IP Address is: " << sf::IpAddress::getPublicAddress() << endl;
}

void Server::MainLoop()
{
    netManager.LaunchThreads();
    running = true;
    sf::Clock clock;
    while (running)
    {
        // Do stuff with all of the received packets
        ProcessPackets();

        // Update the current game state, also send some of this info to the clients
        Update();

        // Calculate elapsed time
        elapsedTime = clock.restart().asSeconds();

        // Sleep some if everything is caught up
        sf::sleep(sf::milliseconds(desiredFrameTime - elapsedTime));
    }
}

void Server::ProcessPackets()
{
    /*
    // Could do something like this, but then process packet would need to do a switch for each packet
    for (int type = 0; type < Packet::PacketTypes; types++)
    {
        while (netManager.ArePackets(type))
            ProcessPacket(netManager.GetPacket(type));
    }
    */
    while (netManager.ArePackets(Packet::ChatMessage))
            ProcessChatMessage(netManager.GetPacket(Packet::ChatMessage));

    while (netManager.ArePackets(Packet::LogIn))
            ProcessLogIn(netManager.GetPacket(Packet::LogIn));
}

void Server::Update()
{
    // TODO: Iterate through the entity grid instead
    entList.Update(elapsedTime);
}

void Server::ProcessChatMessage(sf::Packet& packet)
{
    string msg;
    packet >> msg;
    cout << "Message: " << msg << endl;
    netManager.SendToClients(packet); // TODO: Don't send back to the original sender
}

void Server::ProcessLogIn(sf::Packet& packet)
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
