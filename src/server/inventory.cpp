// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "inventory.h"
#include "packet.h"
#include <iostream>

Inventory::Inventory()
{
    leftSlotId = 0;
    rightSlotId = 0;
}

Inventory::Inventory(unsigned newSize):
    Inventory()
{
    setSize(newSize);
}

void Inventory::setSize(unsigned newSize)
{
    itemSlots.resize(newSize);
}

unsigned Inventory::getSize() const
{
    return itemSlots.size();
}

void Inventory::loadFromConfig(cfg::File& config)
{
    config.useSection("Inventory");
    leftSlotId = config("leftSlotId").toInt();
    rightSlotId = config("rightSlotId").toInt();

    // Load the items from the array
    for (auto& item: config("items"))
        itemSlots.emplace_back(item);

    config.useSection();
}

void Inventory::saveToConfig(cfg::File& config) const
{
    config.useSection("Inventory");
    config("leftSlotId") = leftSlotId;
    config("rightSlotId") = rightSlotId;

    // Save the items to an array
    auto& items = config("items");
    items.clear();
    for (auto& item: itemSlots)
        items.push() = item.toString();

    config.useSection();
}

bool Inventory::addItem(const ItemCode& item)
{
    for (unsigned slotId = 0; slotId < itemSlots.size(); ++slotId)
    {
        if (itemSlots[slotId].isEmpty())
        {
            itemSlots[slotId] = item;
            changedSlots.insert(slotId);
            return true;
        }
    }
    return false;
}

bool Inventory::removeItem(unsigned slotId)
{
    if (slotId < itemSlots.size())
    {
        itemSlots.erase(itemSlots.begin() + slotId);
        changedSlots.insert(slotId);
        return true;
    }
    return false;
}

bool Inventory::changeItem(unsigned slotId, int amount)
{
    if (slotId < itemSlots.size())
    {
        if (amount > 0)
        {
            itemSlots[slotId].amount = amount;
            changedSlots.insert(slotId);
        }
        else
            removeItem(slotId);
        return true;
    }
    return false;
}

const ItemCode& Inventory::getItem(unsigned slotId)
{
    if (slotId >= 0 && slotId < itemSlots.size())
        return itemSlots[slotId];
    else
        return ItemCode::noItem;
}

bool Inventory::swapItems(unsigned slotId1, unsigned slotId2)
{
    if (slotId1 < itemSlots.size() && slotId2 < itemSlots.size() && slotId1 != slotId2)
    {
        std::swap(itemSlots[slotId1], itemSlots[slotId2]);
        changedSlots.insert(slotId1);
        changedSlots.insert(slotId2);
        return true;
    }
    return false;
}

bool Inventory::getChangedItems(sf::Packet& packet)
{
    bool anyChanged = !changedSlots.empty();
    if (anyChanged)
    {
        packet << Packet::InventoryUpdate;
        for (unsigned slotId: changedSlots)
            packet << static_cast<sf::Int32>(slotId) << itemSlots[slotId];
        changedSlots.clear();
    }
    return anyChanged;
}

bool Inventory::getAllItems(sf::Packet& packet) const
{
    bool anyItems = !itemSlots.empty();
    if (anyItems)
    {
        packet << Packet::InventoryUpdate;
        packet << static_cast<sf::Int32>(getSize());
        for (unsigned slotId = 0; slotId < itemSlots.size(); slotId++)
            packet << static_cast<sf::Int32>(slotId) << itemSlots[slotId];
    }
    return anyItems;
}
