// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "inventorygui.h"
#include <cmath>
#include <iostream>
#include "paths.h"

TileSet InventoryGUI::itemTextures(Paths::itemIcons, 8, 8, true);

InventoryGUI::InventoryGUI()
{
    numSlots = 1;
    slotsPerRow = 1;
    inventoryWindow.setFillColor(sf::Color(50, 100, 100, 150));

    inventoryRect.width = 1;
    inventoryRect.height = 1;

    slotHorizontalPadding = 0;
    slotVerticalPadding = 0;

    activeLeftSlot = -1;
    activeRightSlot = -1;

    //Set to false after testing
    visible = true;

    font = nullptr;
}

InventoryGUI::~InventoryGUI()
{
    //dtor
}

//Used to set up the inventory using fractional positions and sizes based on the window size
//Example: inventory.setUp(10, sf::FloatRect(0, .5, .3, .5), font, window) would create an inventory with 10 slots located
//on thhe left side of the screen, halfway down with a width of about 1/3 the window size and a height of half of the
//window height
void InventoryGUI::setUp(int totalSlots, const sf::FloatRect& posSize, sf::Font& descFont, sf::RenderWindow& window)
{
    // TODO: If we won't really need the window passed in, then just pass in an sf::Vector2u of the window size
    inventoryRect.left = posSize.left * window.getSize().x;
    inventoryRect.top = posSize.top * window.getSize().y;
    inventoryRect.width = posSize.width * window.getSize().x;
    inventoryRect.height = posSize.height * window.getSize().y;

    inventoryWindow.setPosition(sf::Vector2f(inventoryRect.left, inventoryRect.top));
    inventoryWindow.setSize(sf::Vector2f(inventoryRect.width, inventoryRect.height));

    font = &descFont;

    setUpSlots(totalSlots);

   // descriptionBox.setupList(window, sf::FloatRect(descriptionBoxLeft, descriptionBoxTop, descriptionBoxWidth, descriptionBoxHeight), descFont, 12, false, true);

}

void InventoryGUI::setUp(int totalSlots, const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& descFont, sf::RenderWindow& window)
{
    inventoryRect.left = pos.x;
    inventoryRect.top = pos.y;
    inventoryRect.width = size.x;
    inventoryRect.height = size.y;

    inventoryWindow.setPosition(pos);
    inventoryWindow.setSize(size);

    font = &descFont;

    setUpSlots(totalSlots);

   // descriptionBox.setupList(window, sf::FloatRect(descriptionBoxLeft, descriptionBoxTop, descriptionBoxWidth, descriptionBoxHeight), descFont, 12, false, true);

}

void InventoryGUI::setUpSlots(int totalSlots)
{
    if (totalSlots <= 0)
        return;

    numSlots = totalSlots;
    slots.resize(totalSlots);

    int slotsPerRow = 1;
    int numRows = 1;

    //Find the area of the inventory
    float area = inventoryRect.width * inventoryRect.height;

    //Divide the area into 'totalSlots' smaller areas
    float slotArea = ceil((area / 1.13)) / (totalSlots);
    //Find the length of the sides of the slot by taking the square root of its area
    float slotSize = sqrt(slotArea);
    //First find how many slots can fit per row and rounding up
    slotsPerRow = ceil(inventoryRect.width / slotSize);

    //Adjust the slot size
    slotSize = ceil((inventoryRect.width / 1.13) / slotsPerRow);

    numRows = floor(totalSlots / slotsPerRow);

    float slotHorizontalSpace = slotSize * slotsPerRow;
    float slotVerticalSpace = slotSize * numRows;

    //Divide the unused horizontal space evenly among the slots per row. +2 to account for the padding at the far left and right
    slotHorizontalPadding = (inventoryRect.width - slotHorizontalSpace) / (slotsPerRow + 1);

    //Evenly divide the unused vertical space evenly amond the rows. +2 to account for the padding at the very top and bottom
    slotVerticalPadding = (inventoryRect.height - slotVerticalSpace) / (numRows + 1);

    std::cout << "Horizontal Padding: " << slotHorizontalPadding << std::endl;
    std::cout << "vertical Padding: " << slotVerticalPadding << std::endl;

    //Make the padding the same on all sides, and make them the smaller of the two calculated sized
    if(slotVerticalPadding < slotHorizontalPadding)
        slotHorizontalPadding = slotVerticalPadding;
    else
        slotVerticalPadding = slotHorizontalPadding;

    //Position the slots
    int row = 0;
    int slot = 0;
    for(int i = 0; i < totalSlots; i++)
    {
        slots[i].setSize(sf::Vector2f(slotSize, slotSize));
        slots[i].setPosition(sf::Vector2f(inventoryRect.left + (slot * slotSize) + (slot + 1) * slotHorizontalPadding, inventoryRect.top + (row * slotSize) + (row + 1) * slotVerticalPadding));
        if (font != nullptr)
            slots[i].setFont(*font);

        slot++;
        if(slot >= slotsPerRow)
        {
            slot = 0;
            row++;
        }
    }

    //float bottomSlotPos = slots.back().getPosition().y + slots.back().getSize().y;

    if (activeLeftSlot >= (int) slots.size())
        activeLeftSlot = -1;
    if (activeRightSlot >= (int) slots.size())
        activeRightSlot = -1;
}

void InventoryGUI::addSlots(int additionalSlots)
{
    if (numSlots + additionalSlots > 0)
    {
        numSlots += additionalSlots;
        setUpSlots(numSlots);
    }
}

void InventoryGUI::setBackgroundColor(const sf::Color& color)
{
    inventoryWindow.setFillColor(color);
}

void InventoryGUI::setBackgroundTexture(sf::Texture& tex)
{
    inventoryWindow.setTexture(&tex);
}

void InventoryGUI::toggleInventory()
{
    visible = !visible;
}

void InventoryGUI::setVisibility(bool visibility)
{
    visible = visibility;
}

bool InventoryGUI::getVisibility() const
{
    return visible;
}

void InventoryGUI::handleMouseMoved(sf::Event event)
{
    if(visible)
    {
        for(unsigned int i = 0; i < slots.size(); i++)
            slots[i].handleMouseMoved(event);
    }
}

void InventoryGUI::handleMouseClicked(sf::Event event)
{
    if (visible)
    {
        for (unsigned int i = 0; i < slots.size(); i++)
        {
            if (slots[i].handleMouseClicked(event))
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Only set the previous slot location to inactive if it existed, or if it is not the same slot (so that it will toggle)
                    if (activeLeftSlot != -1 && activeLeftSlot != (int) i)
                        slots[activeLeftSlot].setInactive();
                    activeLeftSlot = i;
                    if (activeLeftSlot == activeRightSlot) // If you click an already selected slot with the opposite click
                        activeRightSlot = -1; // Reset the old active slot
                    slots[activeLeftSlot].toggleActive(Slot::activeLeft);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (activeRightSlot != -1 && activeRightSlot != (int) i)
                        slots[activeRightSlot].setInactive();
                    activeRightSlot = i;
                    if (activeLeftSlot == activeRightSlot)
                        activeLeftSlot = -1;
                    slots[activeRightSlot].toggleActive(Slot::activeRight);
                }
            }
        }
    }
}

void InventoryGUI::handleResizePacket(sf::Packet& packet)
{
    sf::Int32 newSize;
    if (packet >> newSize)
        setUpSlots(newSize);
}

void InventoryGUI::handleUpdatePacket(sf::Packet& packet)
{
    sf::Int32 slotId, type, amount;
    while (packet >> slotId >> type >> amount)
        updateSlot(slotId, type, amount);
}

void InventoryGUI::updateSlot(unsigned int slotId, int type, int amount)
{
    if (slotId < slots.size())
    {
        if (type >= 0 && type < (int) itemTextures.size())
            slots[slotId].addItem(itemTextures[type]); // Set the item image
        else
            slots[slotId].removeItem(); // Remove the item image
        slots[slotId].setText(amount); // Set the item amount
        slots[slotId].showText(amount > 1); // Only show the text if it is greater than 1
    }
}

void InventoryGUI::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    if(visible)
    {
        window.draw(inventoryWindow);
        //window.draw(descriptionBox);
        for(unsigned int i = 0; i < slots.size(); i++)
            window.draw(slots[i]);
    }
}

