// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "server.h"

using namespace std;

const float Server::desiredFrameTime = 1.0 / 120.0;
const float Server::frameTimeTolerance = -10.0 / 120.0;

const ConfigFile::Section Server::defaultOptions = {
    {"port", Option("1337")},
    {"map", Option("serverdata/maps/2.map")},
    {"maxZombies", Option("20")}
};

Server::Server():
    netManager(accounts, clients, entList),
    packetProcessing(&Server::processAllPackets, this)
{
    setup();
}

void Server::setup()
{
    // First release will be v0.1.0 Dev
    cout << "Undead MMO Server v0.0.11.0 Dev\n\n";
    cout << "The server's LAN IP address is: " << sf::IpAddress::getLocalAddress() << "\n\n";
    //cout << "The server's WAN IP address is: " << sf::IpAddress::getPublicAddress() << endl;

    // Load the config file
    config.setDefaultOptions(defaultOptions);
    config.loadConfigFile("server.cfg");

    // Load the map file (in the future this can also be randomly generated)
    tileMap.loadFromFile(config.getOption("map").asString());

    Entity::setMapSize(tileMap.getWidthPx(), tileMap.getHeightPx());

    // Launch the networking and packet processing threads
    netManager.launchThreads();
    packetProcessing.launch();

    // Spawn some test zombies
    int maxZombies = config.getOption("maxZombies").asInt();
    for (int x = 0; x < maxZombies; x++)
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

void Server::start()
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
        if (sleepTime > frameTimeTolerance)
            sf::sleep(sf::seconds(sleepTime));
        else if (warningTimer.getElapsedTime().asSeconds() >= 5)
        {
            cout << "WARNING: Last frame took " << -sleepTime << " seconds too long! FPS: " << 1.0 / elapsedTime << "\n";
            warningTimer.restart();
        }
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
        // TODO: Figure out a better way to not waste CPU usage than sleeping
        sf::sleep(sf::milliseconds(10));
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
    while (entList.getChangedEntities(changedEntitiesPacket))
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
    //cout << "Received input packet from client #" << packet.sender << endl;
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
                    //cout << "Entity ID: " << playerEnt->getID() << ", Angle: " << angle << ".\n";
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
            case Packet::InputType::UseItem:
                useItem(packet.data, senderClient->pData->inventory, playerEnt);
                break;
            case Packet::InputType::PickupItem:
                pickupItem(senderClient->pData->inventory, playerEnt);
                break;
            case Packet::InputType::DropItem:
                dropItem(packet.data, senderClient->pData->inventory, playerEnt);
                break;
            case Packet::InputType::SwapItem:
                swapItem(packet.data, senderClient->pData->inventory);
                break;
            case Packet::InputType::WieldItem:
                wieldItem(packet.data, senderClient->pData->inventory, playerEnt);
                break;
            default:
                cout << inputCode << " is an unknown or not yet implemented input type.\n";
                break;
        }
    }
}

void Server::useItem(sf::Packet& packet, Inventory& inventory, Entity* playerEnt)
{
    int slotId;
    if (packet >> slotId)
    {
        if (slotId == 0)
            playerEnt->useItem(); // Use wielded item
        //else
            //useItem(inventory.getItem(slotId)); // Use item in inventory
            // Can we use items directly in the inventory? If so, then we should have wieldable and non-wieldable items.
    }
}

void Server::pickupItem(Inventory& inventory, Entity* playerEnt)
{
    Entity* itemToPickup = entList.findCollision(playerEnt); // Find an item you are stepping on
    if (itemToPickup != nullptr)
    {
        // In the future we could always add an auto-wield option to the client which would get sent with this request.
        // It would check if the item was wieldable, and if so, swap it with your currently wielded item.
        if (inventory.addItem(itemToPickup)) // Add the item to your inventory
            entList.erase(itemToPickup->getID()); // Remove the item from the entity list
    }
}

void Server::dropItem(sf::Packet& packet, Inventory& inventory, Entity* playerEnt)
{
    int slotId;
    if (packet >> slotId)
    {
        const ItemCode& itemToDrop = inventory.getItem(slotId); // Get the item code to drop
        if (!itemToDrop.isEmpty()) // If the item slot isn't empty
        {
            if (slotId == 0) // If the item is in slot 0 (the currently wielded slot)
                playerEnt->removeItem(); // Remove the currently wielded item
            Entity* itemOnGround = entList.add(Entity::Item); // Add the item to the entity list
            if (itemOnGround != nullptr)
            {
                itemOnGround->attachItem(itemToDrop); // Set the entity's item code
                inventory.removeItem(slotId); // Remove the item from the inventory
            }
        }
    }
}

void Server::swapItem(sf::Packet& packet, Inventory& inventory)
{
    int slotId1, slotId2;
    if (packet >> slotId1 >> slotId2)
    {
        inventory.swapItems(slotId1, slotId2);
        // May need to re-wield or will the selection boxes also be swapped?
        // It might be better to have the selection boxes be swapped as well,
        // so that you don't accidentally wield your items when organizing your inventory.
    }
}

void Server::wieldItem(sf::Packet& packet, Inventory& inventory, Entity* playerEnt)
{
    int slotId;
    bool primary; // left = true, right = false
    //bool wieldOrUnwield; // Or do we want this function to toggle?
    // Then we should figure out a way to prevent the client from getting out of sync..
    if (packet >> slotId >> primary)
    {
        // Get the item from the inventory
        const ItemCode& itemToWield = inventory.getItem(slotId);
        if (!itemToWield.isEmpty())
            playerEnt->attachItem(itemToWield); // Wield the item
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
                            newPlayer->setPos(sf::Vector2f(pData->positionX, pData->positionY));
                        }
                        cout << "New player entity, ID = " << newPlayerId << endl;
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
                        // Send the map to the player
                        sf::Packet tileMapPacket;
                        tileMap.saveToPacket(tileMapPacket);
                        c->tcpSend(tileMapPacket);
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


