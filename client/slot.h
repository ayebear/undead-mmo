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

        void setPosition(sf::Vector2f);
        void setSize(sf::Vector2f);
        void addItem(sf::Texture&);
        void removeItem();

        void setTextFromString(const std::string&);
        void setTextFromInteger(int);

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        bool handleMouseMoved(sf::Event&, sf::RenderWindow&);
        bool handleMouseClicked(sf::Event&, sf::RenderWindow&);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:

        sf::Color backgroundEmpty;
        sf::Color backgroundNonEmpty;

        sf::RectangleShape slotBackground;
        sf::RectangleShape slot;
        sf::FloatRect slotRect;

        int active;
        bool isEmpty;
        sf::Color activeColors[totalActiveTypes];

        //Number in the bottom left corner of the slot
        bool fontSet;
        bool textVisible;
        short fontSize;
        sf::Text text;

};

#endif // SLOT_H
