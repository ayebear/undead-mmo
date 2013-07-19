#ifndef MISCNETWORK_H
#define MISCNETWORK_H

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

// Used for storing an IP address and port; is also usable in associative containers
struct IpPort
{
    IpPort(): port(Network::clientPort) {}
    bool operator<(const IpPort& addr) const
    {
        if (ip == addr.ip)
            return (port < addr.port);
        else
            return (ip < addr.ip);
    }
    sf::IpAddress ip;
    unsigned short port;
};

#endif
