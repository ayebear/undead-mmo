// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ITEMCODE_H
#define ITEMCODE_H

#include <SFML/Network.hpp>
#include <string>

class ItemCode
{
    public:
        ItemCode();
        ItemCode(int, int);
        ItemCode(const std::string&);

        bool isEmpty() const;
        std::string toString() const;
        void fromString(const std::string&);

        static const sf::Int32 empty = -1;
        static const ItemCode noItem;

        sf::Int32 type; // Represents the type of item
        sf::Int32 amount; // Used for things like ammo
};

sf::Packet& operator <<(sf::Packet&, const ItemCode&);

sf::Packet& operator >>(sf::Packet&, ItemCode&);

#endif
