// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "server.h"
#include "paths.h"
#include <functional>

const float Server::desiredFrameTime = 1.0 / 120.0;
const float Server::frameTimeTolerance = -10.0 / 120.0;

// TODO: Add new server options from GDoc
const cfg::File::ConfigMap Server::defaultOptions = {
{"", {
    {"port", cfg::makeOption(1337, 1, 65536)},
    {"map", cfg::makeOption("serverdata/maps/2.map")},
    {"maxZombies", cfg::makeOption(20, 0)},
    {"showExternalIp", cfg::makeOption(false)},
    {"inventorySize", cfg::makeOption(16, 1, 1000)},
    {"accountsDirectory", cfg::makeOption("serverdata/accounts/")}
}}};

Server::Server():
    config(Paths::serverConfigFile, defaultOptions, cfg::File::Warnings || cfg::File::Errors),
    tcpServer(config("port").toInt()),
    accounts(config("accountsDirectory").toString()),
    players(tcpServer)
{
    using namespace std::placeholders;
    tcpServer.setDisconnectedCallback(std::bind(&Server::logOutClient, this, _1));
    tcpServer.setPacketCallback(std::bind(&Server::processPacket, this, _1, _2));
    setup();
}

void Server::setup()
{
    // First release will be v0.1.0 Dev
    cout << "Undead MMO Server v0.0.16.0 Dev\n\n";
    cout << "The server's local IP address is: " << sf::IpAddress::getLocalAddress() << endl;

    if (config("showExternalIp").toBool())
        cout << "The server's external IP address is: " << sf::IpAddress::getPublicAddress() << endl;
    cout << endl;

    // Load the map file (in the future this can also be randomly generated)
    tileMap.loadFromFile(config("map").toString());

    Entity::setMapSize(tileMap.getWidthPx(), tileMap.getHeightPx());

    inventorySize = config("inventorySize").toInt();

    // Spawn some test zombies
    int maxZombies = config("maxZombies").toInt();
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
    cout << "Running TCP server...\n";
    tcpServer.start();
    cout << "Main thread started.\n";
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
    cout << "Main thread finished.\n";
}

void Server::update()
{
    auto lock = tcpServer.getLock();
    // TODO: Iterate through the entity grid instead
    entList.update(elapsedTime);
    sendChangedEntities();
}

void Server::sendChangedEntities()
{
    // Later this will be client-specific as soon as we have spatial partitioning
    sf::Packet changedEntitiesPacket;
    if (entList.getChangedEntities(changedEntitiesPacket))
        tcpServer.send(changedEntitiesPacket);
}

void Server::processPacket(sf::Packet& packet, int id)
{
    int type = -1;
    packet >> type;
    switch (type)
    {
        case Packet::Input:
            processInputPacket(packet, id);
            break;
        case Packet::ChatMessage:
            processChatMessage(packet, id);
            break;
        case Packet::LogIn:
            processLogIn(packet, id);
            break;
        case Packet::CreateAccount:
            processCreateAccount(packet, id);
            break;
        default:
            cout << "Error: Unknown received packet type. Type = " << type << endl;
            break;
    }
}

void Server::processInputPacket(sf::Packet& packet, int id)
{
    //cout << "Received input packet from client #" << id << endl;
    auto sender = players.getPlayer(id);
    if (sender)
    {
        Entity* playerEnt = entList.find(sender->playerEid);
        int inputCode = -1;
        packet >> inputCode;
        switch (inputCode)
        {
            case Packet::InputType::StartMoving:
            {
                float angle;
                if (packet >> angle)
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
                if (packet >> angle)
                    playerEnt->setVisualAngle(angle);
            }
                break;
            case Packet::InputType::UseItem:
                useItem(packet, sender->playerData.inventory, playerEnt);
                break;
            case Packet::InputType::PickupItem:
                pickupItem(sender->playerData.inventory, playerEnt);
                break;
            case Packet::InputType::DropItem:
                dropItem(packet, sender->playerData.inventory, playerEnt);
                break;
            case Packet::InputType::SwapItem:
                swapItem(packet, sender->playerData.inventory);
                break;
            case Packet::InputType::WieldItem:
                wieldItem(packet, sender->playerData.inventory, playerEnt);
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
                // TODO: Make it so entities can be an item code, but only the item entities will have a function with actual code
                //itemOnGround->attachItem(itemToDrop); // Set the entity's item code
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
            playerEnt->attachItem(itemToWield.type); // Wield the item
    }
}

void Server::processChatMessage(sf::Packet& packet, int id)
{
    int subType = -1;
    if (packet >> subType)
    {
        auto sender = players.getPlayer(id);
        if (sender)
        {
            string msgPrefix = sender->playerData.username + ": ";
            if (subType == Packet::Chat::Public)
            {
                // Process the message
                string msg;
                if (packet >> msg)
                {
                    msg.insert(0, msgPrefix);
                    cout << msg << endl;
                    // Relay the message back to everyone else
                    sf::Packet packetToSend;
                    packetToSend << Packet::ChatMessage << Packet::Chat::Public << msg;
                    tcpServer.send(packetToSend);
                }
            }
            else if (subType == Packet::Chat::Private)
            {
                // Process the username
                string username;
                packet >> username;
                string msg;
                sf::Packet packetToSend;
                auto receiver = players.getPlayer(username);
                if (receiver)
                {
                    // Process the message
                    if (packet >> msg)
                    {
                        msg.insert(0, msgPrefix);
                        cout << "Message to " << username << ": " << msg << endl;

                        // Send the private message
                        packetToSend << Packet::ChatMessage << Packet::Chat::Private << msg;
                        tcpServer.send(packetToSend, receiver->id);

                        // Send a message back to the person who requested to send the message
                        packetToSend.clear();
                        msg = "Message to \"" + username + "\" was successfully sent.";
                        packetToSend << Packet::ChatMessage << Packet::Chat::Server << msg;
                        tcpServer.send(packetToSend, id);
                    }
                }
                else
                {
                    // Send a message back to the person who requested to send the message
                    msg = "Error sending message to \"" + username + "\".";
                    packetToSend << Packet::ChatMessage << Packet::Chat::Server << msg;
                    tcpServer.send(packetToSend, id);
                }
            }
        }
    }
}

void Server::processLogIn(sf::Packet& packet, int id)
{
    int protocolVersion = -1;
    string username, password;
    int loginStatusCode = Packet::LogInCode::UnknownFailure;

    if (packet >> protocolVersion)
    {
        cout << "Packet protocol version extracted.\n";
        if (protocolVersion == Packet::ProtocolVersion)
        {
            cout << "Packet protocol version is correct.\n";
            if (packet >> username >> password)
            {
                cout << "Extracted username and password: " << username << ", " << password << endl;
                if (!players.getPlayer(username)) // Make sure the user is NOT already logged in
                {
                    cout << "User is not already logged in.\n";
                    Player& player = players.addPlayer(id);
                    // Try logging into the account database with the received username and password
                    int dbLogInStatus = accounts.logIn(username, password, player.playerData);
                    if (dbLogInStatus == Packet::LogInCode::Successful)
                    {
                        loginStatusCode = Packet::LogInCode::Successful; // The player has successfully logged in!
                        handleSuccessfulLogIn(player); // Do everything that needs to be done for them to be logged in
                    }
                    else
                        loginStatusCode = dbLogInStatus;
                }
                else
                    loginStatusCode = Packet::LogInCode::AlreadyLoggedIn;
            }
        }
        else
            loginStatusCode = Packet::LogInCode::ProtocolVersionMismatch;
    }

    // Send a packet back to the client with their login status
    sf::Packet loginStatusPacket;
    loginStatusPacket << Packet::LogInStatus << loginStatusCode;
    tcpServer.send(loginStatusPacket, id);

    if (loginStatusCode == Packet::LogInCode::Successful)
    {
        string logInMessage = username + " logged in.";
        //netManager.sendServerChatMessage(logInMessage, id);
        cout << logInMessage << endl;
    }
    else
        cout << "Denied login request. Error code = " << loginStatusCode << endl;
}

void Server::processCreateAccount(sf::Packet& packet, int id)
{
    int protocolVersion = -1;
    int createAccountStatus = Packet::CreateAccountCode::UnknownFailure;

    if (packet >> protocolVersion)
    {
        if (protocolVersion == Packet::ProtocolVersion)
        {
            PlayerData playerData;
            packet >> playerData.username >> playerData.passwordHash;

            cout << "Create account request: " << playerData.username << ", password: " << playerData.passwordHash << endl;

            if (!playerData.username.empty() && !playerData.passwordHash.empty())
            {
                createAccountStatus = accounts.createAccount(playerData);

                // Send a packet back to the client with their create account status
                sf::Packet statusPacket;
                statusPacket << Packet::CreateAccountStatus << createAccountStatus;
                tcpServer.send(statusPacket, id);
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

void Server::handleSuccessfulLogIn(Player& player)
{
    // Set the inventory size if it hasn't been set already (so that different players can have different inventory sizes)
    if (player.playerData.inventory.getSize() <= 0)
        player.playerData.inventory.setSize(inventorySize);
    // Make a new player entity for this player
    Entity* newPlayer = entList.add(Entity::Player);
    EID newPlayerId = 0;
    if (newPlayer != nullptr)
    {
        newPlayerId = newPlayer->getID();
        newPlayer->setPos(sf::Vector2f(player.playerData.positionX, player.playerData.positionY));
    }
    cout << "New player entity, ID = " << newPlayerId << endl;
    player.playerEid = newPlayerId;
    // Send the new player entity ID to the player
    sf::Packet playerIdPacket;
    playerIdPacket << Packet::OnSuccessfulLogIn << newPlayerId;
    tcpServer.send(playerIdPacket, player.id);
    // Send all of the entities to the player
    sf::Packet allEntsPacket;
    if (entList.getAllEntities(allEntsPacket))
        tcpServer.send(allEntsPacket, player.id);
    // Send the map to the player
    sf::Packet tileMapPacket;
    tileMap.saveToPacket(tileMapPacket);
    tcpServer.send(tileMapPacket, player.id);
    // Send the inventory to the player
    sf::Packet inventoryPacket;
    if (player.playerData.inventory.getAllItems(inventoryPacket))
    {
        cout << "Sending items from inventory...\n";
        tcpServer.send(inventoryPacket, player.id);
    }
    cout << "Sent initial packets to " << player.playerData.username << endl;
}

void Server::logOutClient(int id)
{
    auto player = players.getPlayer(id);
    if (player)
    {
        std::string username = player->playerData.username;
        // Save the player's position
        Entity* playerEnt = entList.find(player->playerEid);
        if (playerEnt != nullptr)
        {
            sf::Vector2f pos = playerEnt->getPos(); // Get the position of the player's entity
            player->playerData.positionX = pos.x;
            player->playerData.positionY = pos.y;
            entList.erase(player->playerEid); // Remove the player's entity
        }
        //netManager.sendServerChatMessage(player->playerData.username + " has logged out.", player->id);
        accounts.saveAccount(player->playerData); // Save their account data in the account database
        players.removePlayer(id);
        cout << username << " (" << id << ") logged out.\n";
    }
}
