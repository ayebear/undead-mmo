// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <SFML/Graphics.hpp>
#include <vector>

class TextItem: public sf::Drawable
{
    public:

        TextItem();
        TextItem(const std::string&, sf::Font&, unsigned int, sf::Color, sf::Vector2f);

        ~TextItem();

        void setTextItem(const std::string&, sf::Font&, unsigned int, sf::Color, sf::Vector2f);
        void setTextItem(const std::string&);
        sf::Text getText();

        void setHiddenText(std::string&);
        std::string& getHiddenText();

        void setTextColor(sf::Color);
        void setFontSize(unsigned int);

        sf::Vector2f getTopPosition() const;
        sf::Vector2f getBottomPosition() const;

        //Take the width of the destination box
        void wrapText(sf::FloatRect);

        sf::FloatRect getTextBounds();

        //Toggle
        void toggleHighlight();
        bool getHighlighted();

        //Returns height of the text. Takes wrapping into account.
        float getTextItemHeight();


        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:

        void fixTextItemBox(sf::FloatRect);

        sf::Font textFont;
        unsigned int textFontSize;
        sf::Color textColor;

        bool isHighlighted;

        //Is used to hold the text that is displayed
        //Can hold the server name and description
        std::vector<sf::Text> textItems;

        //Is used to hold the text that is returned when a text item is selected.
        //Can hold a server ip address
        std::string hiddenText;

        sf::RectangleShape textItemBox;


};

#endif // TEXTITEM_H
