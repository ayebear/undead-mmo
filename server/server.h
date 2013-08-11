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
        void processAllPackets();
        void update();

        void sendChangedEntities();

        void processInputPacket(PacketExtra&);
        void processPacket(PacketExtra&);
        void processChatMessage(PacketExtra&);
        void processLogIn(PacketExtra&);
        void processLogOut(PacketExtra&);
        void processCreateAccount(PacketExtra&);

        static const float desiredFrameTime;
        static const float frameTimeTolerance;
        static const ConfigFile::ConfigMap defaultOptions;

        float elapsedTime;
        sf::Clock clock, warningTimer;
        ServerNetwork netManager;
        sf::Thread packetProcessing;
        AccountDb accounts;
        ClientManager clients;
        MasterEntityList entList;
        Map tileMap;
        ConfigFile config;
};

#endif
