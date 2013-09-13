#include "inventory.h"

Inventory::Inventory()
{
}

Inventory::Inventory(unsigned int newSize)
{
    setSize(newSize);
}

void Inventory::setSize(unsigned int size)
{
    itemSlots.resize(size);
}

void Inventory::loadFromConfig(ConfigFile& cfg)
{
    //cfg.getOption()
}

void Inventory::saveToConfig(ConfigFile& cfg) const
{

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
    for (unsigned int slotId: changedSlots)
        packet << itemSlots[slotId];
    changedSlots.clear();
    return anyChanged;
}
