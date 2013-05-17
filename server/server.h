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
        void ProcessPackets();
        void Update();

        void ProcessChatMessage(sf::Packet&);
        void ProcessLogIn(sf::Packet&);

        static const float desiredFrameTime;

        bool running;
        float elapsedTime;
        ServerNetwork netManager;
        MasterEntityList entList;
};

#endif // SERVER_H
