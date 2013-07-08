#ifndef MISCNETWORK_H
#define MISCNETWORK_H

typedef unsigned int ClientID;

struct PacketExtra
{
    PacketExtra() {}
    PacketExtra(const sf::Packet& d, ClientID s): data(d), sender(s) {}
    sf::Packet data;
    ClientID sender;
};

struct IpPort
{
    IpPort(): port(Network::defaultPort) {}
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
