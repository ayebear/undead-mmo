#ifndef IPPORT_H
#define IPPORT_H

#include <SFML/Network.hpp>
#include "network.h"

// Used for storing an IP address and port; is also usable in associative containers
struct IpPort
{
    IpPort(): port(Network::clientPort) {}
    bool operator<(const IpPort& addr) const;
    bool operator==(const IpPort& addr) const;
    sf::IpAddress ip;
    unsigned short port;
};

#endif
