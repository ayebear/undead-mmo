// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef IPPORT_H
#define IPPORT_H

#include <SFML/Network.hpp>
#include "network.h"

// Used for storing an IP address and port; is also usable in associative containers
struct IpPort
{
    IpPort(): port(clientPort) {}
    bool operator<(const IpPort& addr) const;
    bool operator==(const IpPort& addr) const;
    sf::IpAddress ip;
    unsigned short port;
};

#endif
