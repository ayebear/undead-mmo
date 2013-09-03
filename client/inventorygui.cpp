#include "inventorygui.h"
#include <cmath>
#include <iostream>

InventoryGUI::InventoryGUI()
{
    numSlots = 0;
    slotsPerRow = 1;
    inventoryWindow.setFillColor(sf::Color(50, 100, 100, 150));

    inventoryRect.width = 1;
    inventoryRect.height = 1;

    slotRect.width = 1;
    slotRect.height = 1;

    slotHorizontalPadding = 0;
    slotVerticalPadding = 0;

    activeLeftSlot = 0;
    activeRightSlot = 0;

    //Set to false after testing
    visible = true;
}

InventoryGUI::~InventoryGUI()
{
    //dtor
}

void InventoryGUI::setUp(int totalSlots, const sf::FloatRect& posSize, sf::Font& descFont, sf::RenderWindow& window)
{

}

void InventoryGUI::setUp(int totalSlots, const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& descFont, sf::RenderWindow& window)
{
    //Set up the viewport numbers for the description box. The box will occupy the bottom third portion of the inventory window
    float descriptionBoxWidth = size.x / window.getSize().x;
    float descriptionBoxHeight = (size.y / 3) / window.getSize().y;
    float descriptionBoxLeft = pos.x / window.getSize().x;
    float descriptionBoxTop = (pos.y + size.y) / window.getSize().y;
// - (size.y / 3)
    if(totalSlots > 0)
        numSlots = totalSlots;

    slots.resize(numSlots);

    inventoryRect.left = pos.x;
    inventoryRect.top = pos.y;
    inventoryRect.width = size.x;
    inventoryRect.height = size.y;

    inventoryWindow.setPosition(pos);
    inventoryWindow.setSize(size);

    //Slots occupy the upper 2/3 portion of the inventory
    slotWindow.setPosition(pos.x, pos.y);
    slotWindow.setSize(sf::Vector2f(size.x, size.y));
//- (size.y / 3)
    slotRect.left = pos.x;
    slotRect.top = pos.y;
    slotRect.width = size.x;
    slotRect.height = size.y;
//- (size.y / 3)
  //  std::cout << "SlotRect Height " << slotRect.height << std::endl;
   // std::cout << "SlotRect Width " << slotRect.width << std::endl;

    setUpSlots(totalSlots);

   // descriptionBox.setupList(window, sf::FloatRect(descriptionBoxLeft, descriptionBoxTop, descriptionBoxWidth, descriptionBoxHeight), descFont, 12, false, true);


}

void InventoryGUI::setUpSlots(int totalSlots)
{

    int slotsPerRow = 1;
    int numRows = 1;

    //Find the area of the inventory
    float area = slotRect.width * slotRect.height;

    //Divide the area into 'totalSlots' smaller areas
    float slotArea = area / totalSlots;

    //Find the length of the sides of the slot by taking the square root of its area
    float slotSize = sqrt(slotArea);

    //First find how many slots can fit per row and rounding up
    slotsPerRow = ceil(slotRect.width / slotSize);

    //Adjust the slot size
    slotSize = slotRect.width / slotsPerRow / 1.2;

    numRows = totalSlots / slotsPerRow;

    float slotHorizontalSpace = slotSize * slotsPerRow;
    float slotVerticalSpace = slotSize * numRows;

    //Divide the unused horizontal space evenly among the slots per row. +2 to account for the padding at the far left and right
    slotHorizontalPadding = (slotRect.width - slotHorizontalSpace) / (slotsPerRow + 1);

    //Evenly divide the unused vertical space evenly amond the rows. +2 to account for the padding at the very top and bottom
    slotVerticalPadding = (slotRect.height - slotVerticalSpace) / (numRows + 1);

   // std::cout << "Horizontal Padding: " << slotHorizontalPadding << std::endl;
   // std::cout << "vertical Padding: " << slotVerticalPadding << std::endl;

    if(slotVerticalPadding < slotHorizontalPadding)
        slotHorizontalPadding = slotVerticalPadding;
    else
        slotVerticalPadding = slotHorizontalPadding;

    //Position the slots
    int row = 0;
    int slot = 0;
    for(int i = 0; i < totalSlots; i++)
    {
       // std::cout << "Horizontal Padding: " << slotHorizontalPadding << std::endl;
        //std::cout << "vertical Padding: " << slotVerticalPadding << std::endl;
        slots[i].setSize(sf::Vector2f(slotSize, slotSize));
        slots[i].setPosition(sf::Vector2f(slotRect.left + (slot * slotSize) + (slot + 1) * slotHorizontalPadding, slotRect.top + (row * slotSize) + (row + 1) * slotVerticalPadding));

        slot++;
        if(slot >= slotsPerRow)
        {
            slot = 0;
            row++;
        }
    }

}

void InventoryGUI::addSlots(int additionalSlots)
{
    if(numSlots + additionalSlots > 0)
    {
        numSlots += additionalSlots;
        slots.resize(numSlots);
        setUpSlots(numSlots);

        if(activeLeftSlot >= slots.size())
            activeLeftSlot = slots.size() - 1;
        if(activeRightSlot >= slots.size())
            activeRightSlot = slots.size() - 1;
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

void InventoryGUI::handleMouseMoved(sf::Event event, sf::RenderWindow& window)
{
    if(visible)
    {
        for(unsigned int i = 0; i < slots.size(); i++)
            slots[i].handleMouseMoved(event, window);
    }
}

void InventoryGUI::handleMouseClicked(sf::Event event, sf::RenderWindow& window)
{
    if(visible)
    {
        for(unsigned int i = 0; i < slots.size(); i++)
        {
            if(slots[i].handleMouseClicked(event, window))
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    if(activeLeftSlot != i && activeLeftSlot != activeRightSlot)
                        slots[activeLeftSlot].setInactive();
                    activeLeftSlot = i;
                    slots[activeLeftSlot].toggleActive(Slot::activeLeft);
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    if(activeRightSlot != i && activeLeftSlot != activeRightSlot)
                        slots[activeRightSlot].setInactive();
                    activeRightSlot = i;
                    slots[activeRightSlot].toggleActive(Slot::activeRight);
                }
            }
        }
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

