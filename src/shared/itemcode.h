// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ITEMCODE_H
#define ITEMCODE_H

#include <SFML/Network.hpp>
#include <string>

class ItemCode
{
    public:
        static const sf::Int32 empty = -1;
        ItemCode();
        ItemCode(int, int);
        bool isEmpty() const;
        std::string toString() const;
        void fromString(const std::string&);
        sf::Int32 type; // Represents the type of item
        sf::Int32 amount; // Used for things like ammo
        static const ItemCode noItem;
};

sf::Packet& operator <<(sf::Packet&, const ItemCode&);

sf::Packet& operator >>(sf::Packet&, ItemCode&);

#endif
