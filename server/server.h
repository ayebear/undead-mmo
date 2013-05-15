// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <SFML/Network.hpp>
#include "../shared/packet.h"
#include "servernetwork.h"

class Server
{
    public:
        Server();
        void Start();
    private:
        void PrintWelcomeMsg();
        void MainLoop();

        static const float desiredFrameTime;

        bool running;
        float elapsedTime;
        ServerNetwork netManager;
};

#endif // SERVER_H
