// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TEXTITEMLIST_H
#define TEXTITEMLIST_H

#include <SFML/Graphics.hpp>
#include <deque>

#include "scrollbar.h"
#include "textitem.h"

/**
    A scrollable "window" that contains text items.
*/
class TextItemList: public sf::Drawable
{
    public:
        TextItemList();
        ~TextItemList();

        //Used when the itemList needs to be interacted with
        std::string handleEvents(sf::RenderWindow& window);

        void setupList(sf::RenderWindow&, sf::FloatRect, sf::Font&, unsigned int, bool, bool);
        sf::FloatRect getListDimensions();

        void setClickable(bool);
        bool getClickable();

        bool toggleBackground();
        bool isBackgroundVisible();
        bool listContainsMouse(sf::RenderWindow&);

        void addTextItem(const std::string&, const sf::Color& = sf::Color(sf::Color::White));
        void addItemWithHiddenText(const std::string&, const std::string&, const sf::Color& = sf::Color(sf::Color::White));
        void clearList();

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        void scrollToBottom();
        void scrollToTop();
        void scrollDown(unsigned int);
        void scrollUp(unsigned int);

        void setTextFont(std::string&);

        void reposition(float, float);
        void resizeBox(sf::RenderWindow&, sf::FloatRect);

        void handleScrolling(sf::Event&, sf::RenderWindow&);
        std::string& handleMouseClicked(sf::Event&, sf::RenderWindow&);
        void handleResize(sf::Event&, sf::RenderWindow&);

    private:

        sf::Vector2f getNewItemPos();

        sf::View itemListView;

        //Dimensions of the actual list
        sf::FloatRect viewableArea;
        sf::RectangleShape viewableAreaBox;

        ScrollBar scrollBar;
        std::deque<TextItem> textItemList;

        bool isClickable;
        bool isReady;
        bool selectionMade;
        int currentSelection;
        const unsigned int maxTextItems = 150;

        float currViewTop;
        float currViewBot;
        sf::Font* textItemFont;
        unsigned int textFontSize;
        bool backgroundBoxVisible;

};

#endif // TEXTITEMLIST_H
