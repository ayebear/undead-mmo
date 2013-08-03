// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "server.h"

using namespace std;

const float Server::desiredFrameTime = 1.0 / 120.0;

Server::Server():
    netManager(accounts, clients, entList),
    packetProcessing(&Server::processAllPackets, this)
{
}

void Server::start()
{
    cout << "setup()\n";
    setup();
    cout << "mainLoop()\n";
    mainLoop();
    cout << "start() has ended.\n";
}

void Server::setup()
{
    // First release will be v0.1.0 Dev
    cout << "Undead MMO Server v0.0.9.9 Dev\n\n";
    cout << "The server's LAN IP Address is: " << sf::IpAddress::getLocalAddress() << endl;
    //cout << "The server's WAN IP Address is: " << sf::IpAddress::getPublicAddress() << endl;

    Entity::setMapSize(1024, 1024); // Until we get a tile map setup on the server

    netManager.launchThreads();
    packetProcessing.launch();

    // Spawn some test zombies
    for (int x = 0; x < 5; x++)
    {
        auto* zombie = entList.add(Entity::Zombie);
        //zombie->setTexture(zombieTex);
        zombie->setPos(sf::Vector2f(rand() % 1024, rand() % 1024));
        zombie->setAngle(rand() % 360);
        zombie->setMoving(true);
        zombie->setSpeed(rand() % 100 + 50);
        //zombie->moveTo(sf::Vector2f(500, 500));
    }
}

void Server::mainLoop()
{
    while (true)
    {
        // Update the current game state, also send some of this info to the clients
        update();

        // Calculate elapsed time
        elapsedTime = clock.restart().asSeconds();

        // Sleep some if everything is caught up, otherwise skip over this right away
        // (If this is skipped, it means the server is running BELOW the desired FPS)
        float sleepTime = desiredFrameTime - elapsedTime;
        if (sleepTime > 0)
            sf::sleep(sf::seconds(sleepTime));
        //else
            //cout << "WARNING: Server running below desired FPS!\n";
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
    sendChangedEntities();
}

void Server::sendChangedEntities()
{
    // Later this will be client-specific as soon as we have spatial partitioning
    sf::Packet changedEntitiesPacket;
    if (entList.getChangedEntities(changedEntitiesPacket))
        netManager.sendToAllUdp(changedEntitiesPacket);
}

void Server::processPacket(PacketExtra& packet)
{
    int type = packet.type;
    switch (type)
    {
        case Packet::Input:
            processInputPacket(packet);
            break;
        case Packet::ChatMessage:
            processChatMessage(packet);
            break;
        case Packet::LogIn:
            processLogIn(packet);
            break;
        case Packet::LogOut:
            processLogOut(packet);
            break;
        case Packet::CreateAccount:
            processCreateAccount(packet);
            break;
        default:
            cout << "Error: Unknown received packet type. Type = " << type << endl;
            break;
    }
}

void Server::processInputPacket(PacketExtra& packet)
{
    cout << "Received input packet from client #" << packet.sender << endl;
    Client* senderClient = clients.getClientFromId(packet.sender);
    if (senderClient != nullptr && senderClient->loggedIn)
    {
        Entity* playerEnt = entList.find(senderClient->playerEid);
        int inputCode = -1;
        packet.data >> inputCode;
        switch (inputCode)
        {
            case Packet::InputType::StartMoving:
            {
                float angle;
                if (packet.data >> angle)
                {
                    cout << "Entity ID: " << playerEnt->getID() << ", Angle: " << angle << ", Has changed: " << playerEnt->hasChanged() << ".\n";
                    playerEnt->setAngle(angle);
                    playerEnt->setMoving(true);
                }
            }
                break;
            case Packet::InputType::StopMoving:
                playerEnt->setMoving(false);
                break;
            case Packet::InputType::ChangeVisualAngle:
            {
                float angle;
                if (packet.data >> angle)
                    playerEnt->setVisualAngle(angle);
            }
                break;
            default:
                cout << inputCode << " is an unknown or not yet implemented code.\n";
                break;
        }
    }
}

void Server::processChatMessage(PacketExtra& packet)
{
    int subType = -1;
    if (packet.data >> subType)
    {
        Client* senderClient = clients.getClientFromId(packet.sender);
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
                    netManager.sendToAllTcp(packetToSend);
                }
            }
            else if (subType == Packet::Chat::Private)
            {
                // Process the username
                string username;
                packet.data >> username;
                Client* c = clients.getClientFromUsername(username);
                string msg;
                sf::Packet packetToSend;
                if (c != nullptr)
                {
                    // Process the message
                    if (packet.data >> msg)
                    {
                        msg.insert(0, msgPrefix);
                        cout << "Message to " << username << ": " << msg << endl;
                        packetToSend << Packet::ChatMessage << Packet::Chat::Private << msg;
                        //netManager.sendToClientTcp(packetToSend, c->id);
                        c->tcpSend(packetToSend); // Send the private message

                        // Send a message back to the person who requested to send the message
                        packetToSend.clear();
                        msg = "Message to \"" + username + "\" was successfully sent.";
                        packetToSend << Packet::ChatMessage << Packet::Chat::Server << msg;
                        netManager.sendToClientTcp(packetToSend, packet.sender);
                    }
                }
                else
                {
                    // Send a message back to the person who requested to send the message
                    msg = "Error sending message to \"" + username + "\".";
                    packetToSend << Packet::ChatMessage << Packet::Chat::Server << msg;
                    netManager.sendToClientTcp(packetToSend, packet.sender);
                }
            }
        }
    }
}

void Server::processLogIn(PacketExtra& packet)
{
    int protocolVersion = -1;
    string username, password;
    int loginStatusCode = Packet::LogInCode::UnknownFailure;

    if (packet.data >> protocolVersion)
    {
        if (protocolVersion == Packet::ProtocolVersion)
        {
            packet.data >> username >> password;

            cout << "Login attempt from user: " << username << ", password: " << password << endl;

            // Try logging into the account database with the received username and password
            unique_ptr<PlayerData> pData(new PlayerData);
            int dbLogInStatus = accounts.logIn(username, password, *pData);
            if (dbLogInStatus == Packet::LogInCode::Successful)
            {
                bool userLoggedIn = false;
                Client* tmpClient = clients.getClientFromUsername(username);
                if (tmpClient != nullptr)
                    userLoggedIn = tmpClient->loggedIn;
                // Make sure the user is NOT already logged in
                if (!userLoggedIn)
                {
                    loginStatusCode = Packet::LogInCode::Successful; // The client has successfully logged in!
                    Client* c = clients.getClientFromId(packet.sender);
                    if (c != nullptr)
                    {
                        // Make a new player entity for this client
                        Entity* newPlayer = entList.add(Entity::Type::Player);
                        EID newPlayerId = 0;
                        if (newPlayer != nullptr)
                        {
                            newPlayerId = newPlayer->getID();
                            newPlayer->setPos(sf::Vector2f(rand() % 100 + 50, rand() % 100 + 50));
                        }
                        cout << "New player entity, ID = " << newPlayerId << endl;
                        // TODO: Setup the player entity data like position and/or health from the account database
                        // Set the client's username and logged in status, also attach the new entity
                        c->logIn(username, newPlayerId);
                        c->pData.swap(pData);
                        // Send the new player entity ID to the player
                        sf::Packet playerIdPacket;
                        playerIdPacket << Packet::PlayerEntityId << newPlayerId;
                        c->tcpSend(playerIdPacket);
                        // Send all of the entities to the player
                        sf::Packet allEntsPacket;
                        if (entList.getAllEntities(allEntsPacket))
                            c->tcpSend(allEntsPacket);
                    }
                }
                else
                    loginStatusCode = Packet::LogInCode::AlreadyLoggedIn;
            }
            else
                loginStatusCode = dbLogInStatus;
        }
        else
            loginStatusCode = Packet::LogInCode::ProtocolVersionMismatch;
    }

    // Send a packet back to the client with their login status
    sf::Packet loginStatusPacket;
    loginStatusPacket << Packet::LogInStatus << loginStatusCode;
    netManager.sendToClientTcp(loginStatusPacket, packet.sender, false);

    if (loginStatusCode == Packet::LogInCode::Successful)
    {
        string logInMessage = username + " logged in.";
        netManager.sendServerChatMessage(logInMessage, packet.sender);
    }
    else
        cout << "Denied login request. Error code = " << loginStatusCode << endl;
}

void Server::processLogOut(PacketExtra& packet)
{
    cout << "Received logout packet, but these are not handled anymore.\n";
    /*Client* c = netManager.getClientFromId(packet.sender);
    if (c != nullptr)
    {
        string logOutMessage = c->username + " has logged out.";
        c->logOut();
        netManager.sendServerChatMessage(logOutMessage, packet.sender);
        cout << logOutMessage << endl;
    }
    else
        cout << "Client #" << packet.sender << " has logged out.\n";*/
}

void Server::processCreateAccount(PacketExtra& packet)
{
    int protocolVersion = -1;
    int createAccountStatus = Packet::CreateAccountCode::UnknownFailure;

    if (packet.data >> protocolVersion)
    {
        if (protocolVersion == Packet::ProtocolVersion)
        {
            PlayerData pData;
            packet.data >> pData.username >> pData.passwordHash;

            cout << "Create account request: " << pData.username << ", password: " << pData.passwordHash << endl;

            if (!pData.username.empty() && !pData.passwordHash.empty())
            {
                createAccountStatus = accounts.createAccount(pData);

                // Send a packet back to the client with their create account status
                sf::Packet statusPacket;
                statusPacket << Packet::CreateAccountStatus << createAccountStatus;
                netManager.sendToClientTcp(statusPacket, packet.sender, false);
            }
        }
        else
            createAccountStatus = Packet::CreateAccountCode::ProtocolVersionMismatch;
    }

    if (createAccountStatus == Packet::CreateAccountCode::Successful)
        cout << "Account was successfully created!\n";
    else
        cout << "Error: Account was not created. Status code = " << createAccountStatus << endl;
}
