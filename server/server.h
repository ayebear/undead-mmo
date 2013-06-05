// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <SFML/Network.hpp>
#include "../shared/packet.h"
#include "servernetwork.h"
#include "masterentitylist.h"

class Server
{
    public:
        Server();
        void Start();
    private:
        void PrintWelcomeMsg();
        void MainLoop();
        void ProcessAllPackets();
        void Update();

        void ProcessPacket(PacketExtra&);
        void ProcessChatMessage(PacketExtra&);
        void ProcessLogIn(PacketExtra&);

        static const float desiredFrameTime;

        bool running;
        float elapsedTime;
        sf::Clock clock;
        ServerNetwork netManager;
        MasterEntityList entList;
};

#endif
