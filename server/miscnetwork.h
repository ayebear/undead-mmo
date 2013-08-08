#ifndef MISCNETWORK_H
#define MISCNETWORK_H

#include "ipport.h"

typedef unsigned int ClientID;

// Used for storing packets along with necessary information such as the sender and type of packet
struct PacketExtra
{
    PacketExtra(): sender(0), type(0) {}
    PacketExtra(const sf::Packet& d, ClientID s, int t): data(d), sender(s), type(t) {}
    sf::Packet data;
    ClientID sender;
    int type;
};

#endif
