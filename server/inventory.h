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
        Inventory(unsigned int);

        // Inventory functions
        void setCurrentSize(unsigned int);
        unsigned int getCurrentSize() const;
        void setSize(unsigned int);
        unsigned int getSize() const;
        void loadFromConfig(ConfigFile&);
        void saveToConfig(ConfigFile&) const;

        // Item functions
        bool addItem(Entity*); // For picking up an item from the map and adding it to the inventory
        bool addItem(const ItemCode&); // For adding an item to the inventory
        bool removeItem(unsigned int); // For dropping an item - note that other code will need to add a new item entity to the entity list
        bool changeItem(unsigned int, int); // For changing items
        const ItemCode& getItem(unsigned int); // For changing items
        bool swapItems(unsigned int, unsigned int); // For swapping items, like for wielding or just rearranging your inventory
        bool getChangedItems(sf::Packet&); // For the server to send stuff to the client, returns true if anything changed
        bool getAllItems(sf::Packet&); // For the server to send everything to the client, returns true if there are any items
        void getSize(sf::Packet&) const; // For the server to send the inventory to the client

    private:
        std::vector <ItemCode> itemSlots; // Holds all of the items in the inventory. Slot 0 and 1 are for your main items.
        std::map <unsigned int, unsigned int> stackableLookup; // Used for efficient lookup of stackable items and the number of items in each stack
        std::set <unsigned int> changedSlots; // Slot IDs of slots that have been changed. This gets cleared when getChangedItems is called.

        //unsigned int maxSize;
        unsigned int leftSlotId;
        unsigned int rightSlotId;
};

#endif
