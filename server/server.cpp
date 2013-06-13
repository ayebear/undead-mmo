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
    cout << "ZombieSurvivalGame Server v0.3.0.0 Dev\n\n";
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

void Server::ProcessGamePackets()
{
    while (true)
    {
        while (netManager.ArePackets())
        {
            ProcessPacket(netManager.GetPacket());
            netManager.PopPacket();
        }
        //sf::sleep(10);
    }
}

void Server::ProcessOtherPackets()
{
    while (true)
    {
        while (netManager.ArePackets())
        {
            ProcessPacket(netManager.GetPacket());
            netManager.PopPacket();
        }
        //sf::sleep(10);
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
            cout << "Error: Unknown received packet type. Type = " << type << endl;
            break;
    }
}

void Server::ProcessChatMessage(PacketExtra& packet)
{
    string msg;
    packet.data >> msg;
    cout << "Message from " << packet.sender << ": " << msg << endl;
    netManager.SendToAll(packet.data, packet.sender);
}

void Server::ProcessLogIn(PacketExtra& packet)
{
    string username, password;
    // Check if the username exists
    // If it does, then check if the password is correct
    // If it is correct, then send a successful login packet back to that client
    sf::Packet loginStatusPacket;
    loginStatusPacket << Packet::AuthStatus << Packet::Auth::Successful;
    // Otherwise, it should send a packet back denying the login
}
