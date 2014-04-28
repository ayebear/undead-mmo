// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <map>
#include <set>
#include <SFML/Network.hpp>
#include "entity.h"
#include "itemcode.h"
#include "configfile.h"

class Inventory
{
    public:
        Inventory();
        Inventory(unsigned);

        // Inventory functions
        void setSize(unsigned);
        unsigned getSize() const;
        void loadFromConfig(cfg::File&);
        void saveToConfig(cfg::File&) const;

        // Item functions
        bool addItem(Entity*); // For picking up an item from the map and adding it to the inventory
        bool addItem(const ItemCode&); // For adding an item to the inventory
        bool removeItem(unsigned); // For dropping an item - note that other code will need to add a new item entity to the entity list
        bool changeItem(unsigned, int); // For changing items
        const ItemCode& getItem(unsigned); // For changing items
        bool swapItems(unsigned, unsigned); // For swapping items, like for wielding or just rearranging your inventory
        bool getChangedItems(sf::Packet&); // For the server to send stuff to the client, returns true if anything changed
        bool getAllItems(sf::Packet&); // For the server to send everything to the client, returns true if there are any items

    private:
        std::vector <ItemCode> itemSlots; // Holds all of the items in the inventory. Slot 0 and 1 are for your main items.
        std::map <unsigned, unsigned> stackableLookup; // Used for efficient lookup of stackable items and the number of items in each stack
        std::set <unsigned> changedSlots; // Slot IDs of slots that have been changed. This gets cleared when getChangedItems is called.

        unsigned leftSlotId;
        unsigned rightSlotId;
};

#endif
