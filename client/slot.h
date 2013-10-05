// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SLOT_H
#define SLOT_H

#include <SFML/Graphics.hpp>

class Slot: public sf::Drawable
{
    public:

        enum Active
        {
            inActive = 0,
            highlighted,
            activeLeft,
            activeRight,
            totalActiveTypes
        };

        Slot();
        ~Slot();

        void setBackgroundColors(const sf::Color&, const sf::Color&);
        void setEmptyBackgroundColor(const sf::Color&);
        void setNonEmptyBackgroundColor(const sf::Color&);

        void setAllOutlineColors(const sf::Color&, const sf::Color&, const sf::Color&, const sf::Color&);
        void setOutlineColor(int, sf::Color&);

        void setActive(int);
        void toggleActive(int);
        void setInactive();
        void setFont(const sf::Font&);

        void setPosition(const sf::Vector2f&);
        void setSize(const sf::Vector2f&);
        void addItem(sf::Texture&);
        void removeItem();

        void setText(const std::string&);
        void setText(int);

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        bool handleMouseMoved(sf::Event);
        bool handleMouseClicked(sf::Event);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        bool updateHighlighted(int, int);
        void updateOutlineColor();

        sf::Color backgroundEmpty;
        sf::Color backgroundNonEmpty;

        sf::RectangleShape slotBackground;
        sf::RectangleShape slot;
        sf::FloatRect slotRect;

        int active;
        bool isEmpty;
        bool isHighlighted;
        sf::Color activeColors[totalActiveTypes];

        //Number in the bottom left corner of the slot
        bool fontSet;
        bool textVisible;
        short fontSize;
        sf::Text text;

};

#endif // SLOT_H
