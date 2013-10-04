// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "itemcode.h"
#include <sstream>

const ItemCode ItemCode::noItem;

ItemCode::ItemCode():
    type(empty),
    amount(0)
{}

bool ItemCode::isEmpty() const
{
    return (type == empty);
}

std::string ItemCode::toString() const
{
    std::string str;
    str += std::to_string(type) + ' ' + std::to_string(amount);
    return str;
}

void ItemCode::fromString(const std::string& str)
{
    std::istringstream tmpStream(str);
    tmpStream >> type >> amount;
}

sf::Packet& operator <<(sf::Packet& packet, const ItemCode& item)
{
    return packet << item.type << item.amount;
}

sf::Packet& operator >>(sf::Packet& packet, ItemCode& item)
{
    return packet >> item.type >> item.amount;
}
