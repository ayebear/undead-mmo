// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef INVENTORYGUI_H
#define INVENTORYGUI_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "slot.h"
#include "textitemlist.h"
#include "tileset.h"

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

        void handleMouseMoved(sf::Event);
        void handleMouseClicked(sf::Event);

        void handleResizePacket(sf::Packet&);
        void handleUpdatePacket(sf::Packet&);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:

        void setUpSlots(int);
        void updateSlot(unsigned int, int, int);

        static TileSet itemTextures;

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

        int activeLeftSlot;
        int activeRightSlot;

};

#endif // INVENTORYGUI_H
