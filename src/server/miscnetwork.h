// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MISCNETWORK_H
#define MISCNETWORK_H

#include "ipport.h"

// TODO: Either search and replace this for an int, or make some type of class for it so that it can be compared and stuff
typedef int ClientID;

// Used for storing packets along with necessary information such as the sender and type of packet
struct PacketExtra
{
    PacketExtra();
    PacketExtra(const sf::Packet& d, ClientID s, int t);
    void extractType();
    bool isValid() const;

    sf::Packet data;
    ClientID sender;
    int type;
};

#endif
