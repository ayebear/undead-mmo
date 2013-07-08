// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "textitem.h"
#include <iostream>


TextItem::TextItem()
{
    hiddenText = "";
}
TextItem::TextItem(const std::string& text, sf::Font& font, unsigned int fontSize, sf::Color color, sf::Vector2f pos)
{
    textItemBox.setFillColor(sf::Color::Transparent);

    textFont = font;
    textFontSize = fontSize;
    textColor = color;

    textItems.emplace_back(text, font, fontSize);
    textItems.back().setPosition(pos);
    textItems.back().setColor(color);
}


TextItem::~TextItem()
{
    //dtor
}

void TextItem::setTextItem(const std::string& text, sf::Font& font, unsigned int fontSize, sf::Color color, sf::Vector2f pos)
{
    textItemBox.setFillColor(sf::Color::Transparent);

    textFont = font;
    textFontSize = fontSize;
    textColor = color;

    textItems.emplace_back(text, font, fontSize);
    textItems.back().setPosition(pos);
    textItems.back().setColor(color);
}

void TextItem::setTextItem(const std::string& text)
{
    textItems.emplace_back(text,textFont, textFontSize);
}

sf::Text TextItem::getText()
{
    std::string fullTextString("");
    sf::Text fullText;

    for(unsigned int i = 0; i < textItems.size(); i++)
    {
        fullTextString.append(textItems[i].getString());
    }
    fullText.setString(fullTextString);

    return fullText;
}

void TextItem::setHiddenText(std::string& text)
{
    hiddenText = text;
}

std::string& TextItem::getHiddenText()
{
    return hiddenText;
}

//Take the width of the destination box
void TextItem::wrapText(sf::FloatRect destBox)
{
    std::string tempText("");
    std::string tempText2("");
    sf::Vector2f lastLinePos(textItems.back().getPosition());

    int firstCharXPos = textItems.front().findCharacterPos(0).x;
    while(textItems.back().getGlobalBounds().left + textItems.back().getGlobalBounds().width - textItems.back().getGlobalBounds().left > destBox.width - textFontSize)
    {

        std::cout << textItems.back().getGlobalBounds().width << std::endl;
        std::cout << destBox.left + destBox.width << std::endl;
        unsigned int i = 0;
        int endChar = 0;
        int firstCharXPos = textItems.front().findCharacterPos(0).x;

        tempText = textItems.back().getString();

        //Find fist character that is outside of the destination box
        while(i < tempText.length() && textItems.back().findCharacterPos(i).x - firstCharXPos < destBox.width - (2 * textFontSize))
        {
            i++;
            endChar = i;
            //std::cout << textItems.back().findCharacterPos(i).x << std::endl;
        }

        //Decrement index until a space is found.(Don't truncate a word)
        while(i > 0 && tempText[i] != ' ')
            i--;

        //If one word was wider than the destination box, just truncate the word
        if(i == 0)
            i = endChar;

            //Make a new string with only the characters that were outside of the destination box

        tempText2 = tempText.substr(0, i);
        tempText = tempText.substr(i);

        //std::cout << tempText2 << ".\n";
        //std::cout << tempText << ".\n";
        textItems.emplace_back(tempText, textFont, textFontSize);
        textItems[textItems.size() - 2].setString(tempText2);

        //Move the next text line down
        lastLinePos.y += textFontSize + 5;
        textItems.back().setPosition(lastLinePos);
        textItems.back().setColor(textColor);

        tempText = textItems.back().getString();

    }
    fixTextItemBox(destBox);
}

void TextItem::toggleHighlight()
{
    //If text is not already highlighted.
    if(!isHighlighted)
    {
        isHighlighted = true;
        textItemBox.setFillColor(sf::Color(200, 220, 200, 75));
        //std::cout << "Highlighting text\n";
    }

    //If text is already highlights
    else if(isHighlighted)
    {
        isHighlighted = false;
        textItemBox.setFillColor(sf::Color::Transparent);
        //std::cout << "Unhighlighting text\n";
    }
}

bool TextItem::getHighlighted()
{
    return isHighlighted;
}
sf::FloatRect TextItem::getTextBounds()
{
    sf::FloatRect textBounds(textItemBox.getPosition(), textItemBox.getSize());
    return textBounds;
}
float TextItem::getTextItemHeight()
{

    return textItems.back().getPosition().y + textItems.back().getCharacterSize() - textItems[0].getPosition().y + 5;

}

sf::Vector2f TextItem::getTopPosition() const
{
    return textItems[0].getPosition();
}

sf::Vector2f TextItem::getBottomPosition() const
{
    sf::Vector2f bottom;
    bottom.x = textItems.back().getPosition().x;
    bottom.y = textItems.back().getPosition().y + textItems.back().getGlobalBounds().height;
    return bottom;
}

void TextItem::fixTextItemBox(sf::FloatRect destBox)
{
    if(textItems.size() > 0)
    {
        sf::Vector2f boxSize;
        //Make the bounding box the width of the entire string
        boxSize.x = destBox.width ;
        //Make the bounding box height large enough to fit all lines of text
        boxSize.y = getTextItemHeight();

        textItemBox.setPosition(textItems[0].getPosition());
        textItemBox.setSize(boxSize);
    }
}


void TextItem::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(textItemBox);
    for (unsigned int i = 0; i < textItems.size(); i++)
    {
        window.draw(textItems[i]);
    }
}
