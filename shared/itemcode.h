#ifndef ITEMCODE_H
#define ITEMCODE_H

#include <SFML/Network.hpp>
#include <string>

class ItemCode
{
    public:
        static const int empty = -1;
        ItemCode();
        bool isEmpty() const;
        std::string toString() const;
        void fromString(const std::string&);
        int type; // Represents the type of item
        int amount; // Used for things like ammo
        static const ItemCode noItem;
};

sf::Packet& operator <<(sf::Packet&, const ItemCode&);

sf::Packet& operator >>(sf::Packet&, ItemCode&);

#endif
