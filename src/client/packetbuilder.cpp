#include "packetbuilder.h"
#include "packet.h"

PacketBuilder::PacketBuilder(net::Client& client):
    client(client)
{
}

void PacketBuilder::sendChatMessage(const std::string& msg)
{
    if (!msg.empty())
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << Packet::Chat::Public << msg;
        client.send(msgPacket);
    }
}

void PacketBuilder::sendChatMessage(const std::string& msg, const std::string& username)
{
    if (!msg.empty() && !username.empty())
    {
        sf::Packet msgPacket;
        msgPacket << Packet::ChatMessage << Packet::Chat::Private << username << msg;
        client.send(msgPacket);
    }
}
