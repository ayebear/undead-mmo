// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <SFML/Network.hpp>
#include "packet.h"
#include "servernetwork.h"
#include "masterentitylist.h"

class Server
{
    public:
        Server();
        void start();
    private:
        void printWelcomeMsg();
        void mainLoop();
        void processAllPackets();
        void update();

        void processPacket(PacketExtra&);
        void processChatMessage(PacketExtra&);
        void processLogIn(PacketExtra&);

        static const float desiredFrameTime;

        float elapsedTime;
        sf::Clock clock;
        ServerNetwork netManager;
        MasterEntityList entList;
};

#endif
