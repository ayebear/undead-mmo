// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <map>
#include <set>
#include <SFML/Network.hpp>
#include "itemcode.h"
#include "configfile.h"

class Inventory
{
    public:
        Inventory();
        Inventory(unsigned newSize);

        // Inventory functions
        void setSize(unsigned newSize); // Set the maximum number of slots in the inventory
        unsigned getSize() const;
        void loadFromConfig(cfg::File& config);
        void saveToConfig(cfg::File& config) const;

        // Item functions (none of these change the size of the inventory)
        bool addItem(const ItemCode& item); // Adds a new item into the first empty slot
        bool removeItem(unsigned slotId); // Sets a slot to empty
        bool changeItem(unsigned slotId, int amount); // Set an item's amount
        const ItemCode& getItem(unsigned slotId); // Returns an item object in a slot
        bool swapItems(unsigned slotId1, unsigned slotId2); // Swaps two items (for rearrangement or wielding)
        bool getChangedItems(sf::Packet& packet); // For the server to send any changed items to the client, returns true if anything changed
        bool getAllItems(sf::Packet& packet) const; // For the server to send everything to the client, returns true if there are any items

    private:
        std::vector <ItemCode> itemSlots; // Holds all of the items in the inventory. Slot 0 and 1 are for your main items.
        std::set <unsigned> changedSlots; // Slot IDs of slots that have been changed. This gets cleared when getChangedItems is called.
        //std::map <unsigned, unsigned> stackableLookup; // Used for efficient lookup of stackable items and the number of items in each stack

        unsigned leftSlotId;
        unsigned rightSlotId;
};

#endif
