#ifndef INVENTORYGUI_H
#define INVENTORYGUI_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "slot.h"
#include "textitemlist.h"

class InventoryGUI: public sf::Drawable
{
    public:


        enum Mode
        {
            standardMode = 0,
            dropMode,
            swapMode,
            totalModes
        };

        InventoryGUI();
        ~InventoryGUI();

        const int MINSLOTSIZE = 32;
        const int MAXSLOTSIZE = 128;

        void setUp(int, const sf::FloatRect&, sf::Font&, sf::RenderWindow&);
        void setUp(int, const sf::Vector2f&, const sf::Vector2f&, sf::Font&, sf::RenderWindow&);

        void setBackgroundColor(const sf::Color&);
        void setBackgroundTexture(sf::Texture&);

        void toggleInventory();
        void setVisibility(bool);
        bool getVisibility() const;

        void addSlots(int);

        void handleMouseMoved(sf::Event event, sf::RenderWindow&);
        void handleMouseClicked(sf::Event event, sf::RenderWindow&);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:

        void setUpSlots(int);

        int numSlots;
        int slotsPerRow;
        bool visible;
        sf::Font* font;

        int slotHorizontalPadding;
        int slotVerticalPadding;

        //Entire inventory
        sf::RectangleShape inventoryWindow;
        sf::FloatRect inventoryRect;

        TextItemList descriptionBox;
        std::vector<Slot> slots;

        unsigned int activeLeftSlot;
        unsigned int activeRightSlot;



};

#endif // INVENTORYGUI_H
