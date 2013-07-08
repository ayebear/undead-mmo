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

void Server::start()
{
    printWelcomeMsg();
    mainLoop();
}

void Server::printWelcomeMsg()
{
    cout << "ZombieSurvivalGame Server v0.3.2.0 Dev\n\n";
    cout << "The server's LAN IP Address is: " << sf::IpAddress::getLocalAddress() << endl;
    cout << "The server's WAN IP Address is: " << sf::IpAddress::getPublicAddress() << endl;
}

void Server::mainLoop()
{
    netManager.launchThreads();
    sf::Thread packetProcessing(&Server::processAllPackets, this);
    packetProcessing.launch();

    while (true)
    {
        // Update the current game state, also send some of this info to the clients
        update();

        // Calculate elapsed time
        elapsedTime = clock.restart().asSeconds();

        // Sleep some if everything is caught up, otherwise skip over this right away
        sf::sleep(sf::milliseconds(desiredFrameTime - elapsedTime));
    }
}

void Server::processAllPackets()
{
    while (true)
    {
        while (netManager.arePackets())
        {
            processPacket(netManager.getPacket());
            netManager.popPacket();
        }
        sf::sleep(sf::milliseconds(100));
    }
}

/*
// We could use this in a separate thread later if there are really a lot of packets being processed that are separate from the game state
void Server::ProcessOtherPackets()
{
    while (true)
    {
        while (netManager.arePackets())
        {
            processPacket(netManager.getPacket());
            netManager.popPacket();
        }
        //sf::sleep(10);
    }
}
*/

void Server::update()
{
    // TODO: Iterate through the entity grid instead
    entList.update(elapsedTime);
}

void Server::processPacket(PacketExtra& packet)
{
    int type = 1;
    packet.data >> type;
    switch (type)
    {
        case Packet::ChatMessage:
            processChatMessage(packet);
            break;
        case Packet::LogIn:
            processLogIn(packet);
            break;
        default:
            cout << "Error: Unknown received packet type. Type = " << type << endl;
            break;
    }
}

void Server::processChatMessage(PacketExtra& packet)
{
    string msg;
    packet.data >> msg;
    cout << "Message from " << packet.sender << ": " << msg << endl;
    netManager.sendToAll(packet.data, packet.sender);
}

void Server::processLogIn(PacketExtra& packet)
{
    string username, password;
    // Check if the username exists
    // If it does, then check if the password is correct
    // If it is correct, then send a successful login packet back to that client
    sf::Packet loginStatusPacket;
    loginStatusPacket << Packet::AuthStatus << Packet::Auth::Successful;
    // Otherwise, it should send a packet back denying the login
}
