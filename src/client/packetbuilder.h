#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include <string>
#include "client.h"

/*
This class handles the game specific networking for creating and sending packets.
    It contains convenience functions that allow other classes to easily send packets.
*/
class PacketBuilder
{
    public:
        PacketBuilder(net::Client& client);

        // Packet factories
        void sendChatMessage(const std::string&);
        void sendChatMessage(const std::string&, const std::string&);

    private:

        net::Client& client;
};

#endif
