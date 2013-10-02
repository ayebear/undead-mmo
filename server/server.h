// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <SFML/Network.hpp>
#include "packet.h"
#include "servernetwork.h"
#include "masterentitylist.h"
#include "accountdb.h"
#include "map.h"
#include "configfile.h"

class Server
{
    public:
        Server();
        void start();

    private:
        void setup();
        void receiveUdp();
        void receiveTcp();
        void update();
        void sendChangedEntities();

        // Packet handlers
        void processInputPacket(PacketExtra&);
        void processPacket(PacketExtra&);
        void processChatMessage(PacketExtra&);
        void processLogIn(PacketExtra&);
        void processLogOut(PacketExtra&);
        void processCreateAccount(PacketExtra&);

        void handleSuccessfulLogIn(Client*);

        // Inventory/item functions
        void useItem(sf::Packet&, Inventory&, Entity*);
        void pickupItem(Inventory&, Entity*);
        void dropItem(sf::Packet&, Inventory&, Entity*);
        void swapItem(sf::Packet&, Inventory&);
        void wieldItem(sf::Packet&, Inventory&, Entity*);

        static const float desiredFrameTime;
        static const float frameTimeTolerance;
        static const ConfigFile::Section defaultOptions;

        float elapsedTime;
        sf::Clock clock, warningTimer;
        ServerNetwork netManager;
        sf::Thread udpThread;
        sf::Thread tcpThread;
        AccountDb accounts;
        ClientManager clients;
        MasterEntityList entList;
        Map tileMap;
        ConfigFile config;
        mutex processPacketMutex;
        PacketExtra udpPacket;
        PacketExtra tcpPacket;
        unsigned int inventorySize;
};

#endif
