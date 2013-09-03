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

        void setBackgroundColors(sf::Color&, sf::Color&);
        void setEmptyBackgroundColor(sf::Color&);
        void setNonEmptyBackgroundColor(sf::Color&);

        void setAllOutlineColors(sf::Color&, sf::Color&, sf::Color&, sf::Color&);
        void setOutlineColor(int, sf::Color&);

        void setActive(int);
        void toggleActive(int);
        void setInactive();

        void setPosition(sf::Vector2f);
        void setSize(sf::Vector2f);
        void addItem(sf::Texture&);
        void removeItem();

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

};

#endif // SLOT_H
