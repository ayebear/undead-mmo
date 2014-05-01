// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <SFML/Network.hpp>
#include "packet.h"
#include "masterentitylist.h"
#include "accountdb.h"
#include "../graphics/tilemap.h"
#include "configfile.h"
#include "tcpserver.h"
#include "playermanager.h"

class Server
{
    public:
        Server();
        void start();

    private:
        void setup();
        void update();
        void sendChangedEntities();

        // Packet handlers
        void processPacket(sf::Packet& packet, int id);
        void processInputPacket(sf::Packet& packet, int id);
        void processChatMessage(sf::Packet& packet, int id);
        void processLogIn(sf::Packet& packet, int id);
        void processCreateAccount(sf::Packet& packet, int id);

        // Inventory/item functions
        void useItem(sf::Packet&, Inventory&, Entity*);
        void pickupItem(Inventory&, Entity*);
        void dropItem(sf::Packet&, Inventory&, Entity*);
        void swapItem(sf::Packet&, Inventory&);
        void wieldItem(sf::Packet&, Inventory&, Entity*);

        // Other functions
        void handleSuccessfulLogIn(Player& player);
        void handleClientConnected(int id);
        void logOutClient(int id);

        static const float desiredFrameTime;
        static const float frameTimeTolerance;
        static const cfg::File::ConfigMap defaultOptions;

        float elapsedTime;
        sf::Clock clock, warningTimer;
        cfg::File config;

        // Networking
        //ServerNetwork netManager;
        net::TcpServer tcpServer;
        AccountDb accounts;
        PlayerManager players;

        // The instance of the game
        MasterEntityList entList;
        TileMap tileMap;
        unsigned int inventorySize;
};

#endif
