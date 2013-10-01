// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "miscnetwork.h"
#include "packet.h"

PacketExtra::PacketExtra():
    sender(0),
    type(0)
{
}

PacketExtra::PacketExtra(const sf::Packet& d, ClientID s, int t):
    data(d),
    sender(s),
    type(t)
{
}

void PacketExtra::extractType()
{
    type = -1;
    data >> type;
}

bool PacketExtra::isValid() const
{
    return (sender != -1 && type >= 0 && type < Packet::TotalPacketTypes);
}
