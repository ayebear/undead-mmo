// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "textitemlist.h"
#include "../shared/other.h"
#include <iostream>

TextItemList::TextItemList()
{
    isReady = false;
}

TextItemList::~TextItemList()
{
    //dtor
}


void TextItemList::setupList(sf::RenderWindow& window, sf::FloatRect dimensions, sf::Font& font, unsigned int fontSize, bool clickable)
{

    textItemFont = &font;

    textFontSize = fontSize;

    sf::Vector2f windowSize;
    windowSize.x = window.getSize().x;
    windowSize.y = window.getSize().y;

    viewableArea = dimensions;

    //Set up the view and viewPort
    sf::FloatRect viewPort;
    viewPort.left = dimensions.left / windowSize.x;
    viewPort.top = dimensions.top / windowSize.y;
    viewPort.width = dimensions.width / windowSize.x;
    viewPort.height = dimensions.height / windowSize.y;

    itemListView.reset(viewableArea);

    itemListView.setViewport(viewPort);

    //Box that the user can see
    viewableAreaBox.setPosition(dimensions.left, dimensions.top);
    viewableAreaBox.setSize(sf::Vector2f(dimensions.width, dimensions.height));
    viewableAreaBox.setFillColor(sf::Color(0, 0, 0, 75));

    scrollBar.attachScrollBar(dimensions);

    isClickable = clickable;
    currentSelection = 0;
    isReady = true;
}



void resizeBox(sf::RenderWindow& window, sf::FloatRect dimensions)
{

}

sf::FloatRect TextItemList::getListDimensions()
{
    return viewableArea;
}

void TextItemList::setClickable(bool clickable)
{
    isClickable = clickable;
}
bool TextItemList::getClickable()
{
    return isClickable;
}

void TextItemList::addTextItem(const std::string& newText, const sf::Color& color)
{
    if(isReady)
    {
        sf::Text text(newText, *textItemFont, textFontSize);
        sf::Vector2f newPos = getNewItemPos();


        TextItem newItem(newText, *textItemFont,textFontSize, color, newPos);
        textItemList.push_back(newItem);

        //Wrap the text as needed
        textItemList.back().wrapText(viewableArea);

        //If there are selections. Default to the most recent item
        if(isClickable)
        {
            //Unhighlight the last item if it was already highlighted
            if(textItemList[currentSelection].getHighlighted())
                textItemList[currentSelection].toggleHighlight();

            currentSelection = textItemList.size() - 1;

            //Highlight the new item
            textItemList[currentSelection].toggleHighlight();
        }


        scrollDown(textItemList.back().getTextItemHeight());

        scrollBar.adjustScrollerHeight(viewableArea.height, textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y);


    }

}

sf::Vector2f TextItemList::getNewItemPos()
{
    sf::Vector2f pos(0,0);

    if(textItemList.size() > 0)
    {
         pos = textItemList.back().getBottomPosition();
         pos.x = viewableArea.left + scrollBar.scrollBarWidth + 3;
         pos.y = pos.y + 3;
    }
    else
    {
        pos.x = viewableArea.left + scrollBar.scrollBarWidth + 3;
        pos.y = viewableArea.top + 3;
    }

    return pos;

}

void TextItemList::scrollDown(unsigned int distance)
{
    if(!textItemList.empty())
    {
        float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
        scrollBar.scrollDown(itemListView, actualHeight, textItemList.back().getBottomPosition(), distance);
    }


  //  itemListView.reset(viewableArea);
}
void TextItemList::scrollUp(unsigned int distance)
{
    if(!textItemList.empty())
    {
        float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
        scrollBar.scrollUp(itemListView, actualHeight,  textItemList.front().getTopPosition(), distance);
    }
   // itemListView.reset(viewableArea);
}

void TextItemList::setTextFont(std::string& fontFile)
{
    if (!textItemFont->loadFromFile(fontFile))
        exit(Errors::Font);
}


void TextItemList::handleScrolling(sf::Event& event, sf::RenderWindow& window)
{
    sf::Vector2f mousePos;
    mousePos.x = sf::Mouse::getPosition(window).x;
    mousePos.y = sf::Mouse::getPosition(window).y;
    if(viewableArea.contains(mousePos))
    {
            if(event.mouseWheel.delta > 0)
            {
                for(int i = 0; i < event.mouseWheel.delta; i++)
                    scrollUp(textFontSize);
            }
            else
            {
                for(int i = 0; i > event.mouseWheel.delta; i--)
                {
                    scrollDown(textFontSize);
                }
            }
    }
}
void TextItemList::handleMouseClicked(sf::Event& event, sf::RenderWindow& window)
{
    //Get the mouse coordinates relative to the window to determine if it's inside of the item list
    sf::Vector2f mousePos;
    mousePos.x = sf::Mouse::getPosition(window).x;
    mousePos.y = sf::Mouse::getPosition(window).y;

    if(isClickable && viewableArea.contains(mousePos))
    {
        //Get the mouse coordinates relative to the message box view to determine which item is selected
        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window),itemListView);

        uint i = 0;
        while(i < textItemList.size())
        {

            if(textItemList[i].getTextBounds().contains(mousePos) && event.mouseButton.button == sf::Mouse::Left)
            {
           //     std::cout << "Mouse x: " << mousePos.x << std::endl;
            //    std::cout << "Mouse y: " << mousePos.y << std::endl;

                //Unhighlight the last selection
                textItemList[currentSelection].toggleHighlight();

                //Highlight the new selection
                currentSelection = i;
                textItemList[currentSelection].toggleHighlight();
            }
            i++;
        }
    }
}
void TextItemList::handleResize(sf::Event&, sf::RenderWindow& window)
{

}

void TextItemList::draw(sf::RenderTarget& window, sf::RenderStates states) const
{

    window.draw(viewableAreaBox);
    window.draw(scrollBar);
    window.setView(itemListView);

    //Only draw text that is inside of the viewable area
    for (auto& textItem: textItemList)
    {
        //If the bottom of the view is greater than or equal to the bottom of the text item
        if(itemListView.getCenter().y + itemListView.getSize().y / 2 >= textItem.getTopPosition().y)
            window.draw(textItem);
    }
}

