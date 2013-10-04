// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "inventory.h"
#include "packet.h"

Inventory::Inventory()
{
}

Inventory::Inventory(unsigned int newSize)
{
    setSize(newSize);
}

void Inventory::setCurrentSize(unsigned int size)
{
    itemSlots.resize(size);
}

unsigned int Inventory::getCurrentSize() const
{
    return itemSlots.size();
}

void Inventory::setSize(unsigned int size)
{
    maxSize = size;
}

unsigned int Inventory::getSize() const
{
    return maxSize;
}

void Inventory::loadFromConfig(ConfigFile& cfg)
{
    cfg.setSection("Inventory");
    setSize(cfg["Size"].asInt());
    leftSlotId = cfg["LeftSlotId"].asInt();
    rightSlotId = cfg["RightSlotId"].asInt();
    for (unsigned int i = 0; i < itemSlots.size(); i++) // Loop through all of the item slots in the config file
    {
        itemSlots[i].fromString(cfg[std::to_string(i)].asString()); // Read the option as a string and convert it to an item code
        changedSlots.insert(i); // Add the slot id as a changed slot so that the first call to getChangedItems will return the whole inventory
    }
    cfg.setSection();
}

void Inventory::saveToConfig(ConfigFile& cfg) const
{
    cfg.setSection("Inventory");
    cfg["Size"].set(getSize());
    cfg["LeftSlotId"].set(leftSlotId);
    cfg["RightSlotId"].set(rightSlotId);
    for (unsigned int i = 0; i < itemSlots.size(); i++) // Loop through all of the item slots
        cfg[std::to_string(i)].setString(itemSlots[i].toString()); // Convert the slot ID and item codes to strings and save them to the config file
    cfg.setSection();
}

bool Inventory::addItem(Entity* ent)
{
    return addItem(ent->getItem());
}

bool Inventory::addItem(const ItemCode& item)
{
    itemSlots.push_back(item);
    return true;
}

bool Inventory::removeItem(unsigned int slotId)
{
    itemSlots.erase(itemSlots.begin() + slotId);
    return true;
}

bool Inventory::changeItem(unsigned int slotId, int amount)
{
    itemSlots[slotId].amount = amount;
    return true;
}

const ItemCode& Inventory::getItem(unsigned int slotId)
{
    if (slotId >= 0 && slotId < itemSlots.size())
        return itemSlots[slotId];
    else
        return ItemCode::noItem;
}

bool Inventory::swapItems(unsigned int slotId1, unsigned int slotId2)
{
    std::swap(itemSlots[slotId1], itemSlots[slotId2]);
    return true;
}

bool Inventory::getChangedItems(sf::Packet& packet)
{
    bool anyChanged = !changedSlots.empty();
    if (anyChanged)
    {
        packet << Packet::InventoryUpdate;
        for (unsigned int slotId: changedSlots)
            packet << itemSlots[slotId];
        changedSlots.clear();
    }
    return anyChanged;
}

void Inventory::getSize(sf::Packet& packet) const
{
    packet << Packet::InventoryResize << getSize();
}
