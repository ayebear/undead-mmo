// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

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
    cout << "Undead MMO Server v0.3.4.1 Dev\n\n";
    cout << "The server's LAN IP Address is: " << sf::IpAddress::getLocalAddress() << endl;
    //cout << "The server's WAN IP Address is: " << sf::IpAddress::getPublicAddress() << endl;
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
        sf::sleep(sf::seconds(desiredFrameTime - elapsedTime));
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
    int type = packet.type;
    switch (type)
    {
        case Packet::ChatMessage:
            processChatMessage(packet);
            break;
        case Packet::LogIn:
            processLogIn(packet);
            break;
        case Packet::LogOut:
            processLogOut(packet);
            break;
        default:
            cout << "Error: Unknown received packet type. Type = " << type << endl;
            break;
    }
}

void Server::processChatMessage(PacketExtra& packet)
{
    int subType = -1;
    if (packet.data >> subType)
    {
        Client* senderClient = netManager.getClientFromId(packet.sender);
        if (senderClient != nullptr)
        {
            string msgPrefix = senderClient->username + ": ";
            if (subType == Packet::Chat::Public)
            {
                // Process the message
                string msg;
                if (packet.data >> msg)
                {
                    msg.insert(0, msgPrefix);
                    cout << msg << endl;
                    // Relay the message back to everyone else
                    sf::Packet packetToSend;
                    packetToSend << Packet::ChatMessage << Packet::Chat::Public << msg;
                    netManager.sendToAllTcp(packetToSend, packet.sender);
                }
            }
            else if (subType == Packet::Chat::Private)
            {
                // Process the username
                string username;
                packet.data >> username;
                Client* c = netManager.getClientFromUsername(username);
                if (c != nullptr)
                {
                    // Process the message
                    string msg;
                    if (packet.data >> msg)
                    {
                        msg.insert(0, msgPrefix);
                        cout << "Message to " << username << ": " << msg << endl;
                        sf::Packet packetToSend;
                        packetToSend << Packet::ChatMessage << Packet::Chat::Private << msg;
                        //netManager.sendToClientTcp(packetToSend, c->id);
                        c->tcpSend(packetToSend);
                    }
                }
            }
        }
    }
}

void Server::processLogIn(PacketExtra& packet)
{
    string username, password;
    packet.data >> username >> password;

    cout << "Login attempt from user: " << username << ", password: " << password << endl;

    bool successfulLogin = false;
    sf::Packet loginStatusPacket;
    // Check if the username exists
    // If it does, then check if the password is correct
    // TODO: Use AccountDb class instead!!!
    if (username == "test" && password == "password")
    {
        // Make sure the user is NOT already logged in
        if (netManager.getClientFromUsername(username) == nullptr)
        {
            successfulLogin = true;
            loginStatusPacket << Packet::LoginStatus << Packet::Login::Successful; // If it is correct, then send a successful login packet back to that client
            Client* c = netManager.getClientFromId(packet.sender);
            if (c != nullptr)
            {
                // Set the client's username and logged in status
                c->username = username;
                c->loggedIn = true;
            }
        }
        else
            loginStatusPacket << Packet::LoginStatus << Packet::Login::AlreadyLoggedIn;
    }
    else
        loginStatusPacket << Packet::LoginStatus << Packet::Login::InvalidPassword; // Otherwise, send a packet back denying the login

    netManager.sendToClientTcp(loginStatusPacket, packet.sender, false);

    if (successfulLogin)
    {
        string loginMessage = username + " logged in.";
        sf::Packet packetToSend;
        packetToSend << Packet::ChatMessage << Packet::Chat::Server << loginMessage;
        netManager.sendToAllTcp(packetToSend, packet.sender);
        cout << loginMessage << endl;
    }
}

void Server::processLogOut(PacketExtra& packet)
{
    Client* c = netManager.getClientFromId(packet.sender);
    if (c != nullptr)
    {
        c->logOut();
        cout << "Client #" << packet.sender << " has logged out.\n";
    }
}
