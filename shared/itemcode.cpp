#include "itemcode.h"

const ItemCode ItemCode::noItem;

ItemCode::ItemCode():
    type(empty),
    amount(0)
{}

bool ItemCode::isEmpty() const
{
    return (type == empty);
}

sf::Packet& operator <<(sf::Packet& packet, const ItemCode& item)
{
    return packet << item.type << item.amount;
}

sf::Packet& operator >>(sf::Packet& packet, ItemCode& item)
{
    return packet >> item.type >> item.amount;
}
