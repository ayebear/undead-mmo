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

void Inventory::setSize(unsigned size)
{
    itemSlots.resize(size);
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

bool Inventory::addItem(Entity* ent)
{
    return addItem(ItemCode(ent->getItem(), 1));
}

bool Inventory::addItem(const ItemCode& item)
{
    itemSlots.push_back(item);
    return true;
}

bool Inventory::removeItem(unsigned slotId)
{
    itemSlots.erase(itemSlots.begin() + slotId);
    return true;
}

bool Inventory::changeItem(unsigned slotId, int amount)
{
    itemSlots[slotId].amount = amount;
    return true;
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
    std::swap(itemSlots[slotId1], itemSlots[slotId2]);
    return true;
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

bool Inventory::getAllItems(sf::Packet& packet)
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
