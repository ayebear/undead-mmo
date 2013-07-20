// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "textitemlist.h"
#include <iostream>

TextItemList::TextItemList()
{
    isReady = false;
}

TextItemList::~TextItemList()
{
    //dtor
}


void TextItemList::setupList(sf::RenderWindow& window, sf::FloatRect sizeFactor, sf::Font& font, unsigned int fontSize, bool clickable, bool backgroundVisible)
{

    textItemFont = &font;

    textFontSize = fontSize;

    sf::Vector2f windowSize;
    windowSize.x = window.getSize().x;
    windowSize.y = window.getSize().y;


    //Set up the view and viewPort
    sf::FloatRect viewPort;
    viewPort.left = sizeFactor.left;
    viewPort.top = sizeFactor.top;
    viewPort.width = sizeFactor.width;
    viewPort.height = sizeFactor.height;


    //Box that the user can see
    viewableAreaBox.setPosition(sizeFactor.left * window.getSize().x, sizeFactor.top * window.getSize().y);
    viewableAreaBox.setSize(sf::Vector2f(sizeFactor.width * window.getSize().x, sizeFactor.height * window.getSize().y));
    viewableAreaBox.setFillColor(sf::Color(0, 0, 0, 75));

    viewableArea.left = sizeFactor.left * window.getSize().x;
    viewableArea.top = sizeFactor.top * window.getSize().y;
    viewableArea.width = sizeFactor.width * window.getSize().x;
    viewableArea.height = sizeFactor.height * window.getSize().y;

    itemListView.reset(viewableArea);
    itemListView.setViewport(viewPort);

    scrollBar.attachScrollBar(viewableArea);

    currViewTop = viewableArea.top;
    currViewBot = viewableArea.top + viewableArea.height;

    isClickable = clickable;
    currentSelection = 0;
    selectionMade = false;

    backgroundBoxVisible = backgroundVisible;

    isReady = true;
}


bool TextItemList::toggleBackground()
{
    if (backgroundBoxVisible)
        backgroundBoxVisible = false;
    else
        backgroundBoxVisible = true;
    return backgroundBoxVisible;
}

bool TextItemList::isBackgroundVisible()
{
    return backgroundBoxVisible;
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

        //If more than 150 items are added. Start removing items starting with the oldest one
        if(textItemList.size() > maxTextItems)
            textItemList.pop_front();

        //Wrap the text as needed
        textItemList.back().wrapText(viewableArea);


        scrollDown(textItemList.back().getTextItemHeight());

  //      scrollBar.adjustScrollerHeight(viewableArea.height, textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y);

    }
    else
    {
        std::cout << "ItemList must be set up before using.\n";
    }

}

void TextItemList::clearList()
{
    textItemList.clear();
}

sf::Vector2f TextItemList::getNewItemPos()
{
    sf::Vector2f pos(0,0);

    if(textItemList.size() > 0)
    {
         pos = textItemList.back().getBottomPosition();
         pos.x = viewableAreaBox.getPosition().x + scrollBar.scrollBarWidth + 3;
         pos.y = pos.y + 5;
    }
    else
    {
        pos.x = viewableAreaBox.getPosition().x + scrollBar.scrollBarWidth + 3;
        pos.y = viewableArea.top + 5;
    }

    return pos;

}

void TextItemList::scrollToTop()
{

    float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
    scrollBar.scrollToTop(itemListView, actualHeight, textItemList.front().getTopPosition());

    currViewTop = itemListView.getCenter().y - itemListView.getSize().y / 2;
    currViewBot = itemListView.getCenter().y - itemListView.getSize().y / 2;

    viewableAreaBox.setPosition(viewableAreaBox.getPosition().x, currViewTop);

}

void TextItemList::scrollToBottom()
{

    float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
    scrollBar.scrollToBottom(itemListView, actualHeight, textItemList.back().getBottomPosition());

    currViewTop = itemListView.getCenter().y - itemListView.getSize().y / 2;
    currViewBot = itemListView.getCenter().y - itemListView.getSize().y / 2;

    viewableAreaBox.setPosition(viewableAreaBox.getPosition().x, currViewTop);

}

void TextItemList::scrollDown(unsigned int distance)
{
    if(!textItemList.empty())
    {
        float amountScrolled = 0;
        float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
        amountScrolled = scrollBar.scrollDown(itemListView, actualHeight, textItemList.back().getBottomPosition(), distance);

        currViewTop += amountScrolled;
        currViewBot += amountScrolled;

        viewableAreaBox.setPosition(viewableAreaBox.getPosition().x, currViewTop);

    }


}
void TextItemList::scrollUp(unsigned int distance)
{
    if(!textItemList.empty())
    {
        float amountScrolled = 0;

        float actualHeight = textItemList.back().getBottomPosition().y - textItemList.front().getTopPosition().y;
        amountScrolled = scrollBar.scrollUp(itemListView, actualHeight,  textItemList.front().getTopPosition(), distance);

        currViewTop -= amountScrolled;
        currViewBot-= amountScrolled;

        viewableAreaBox.setPosition(viewableAreaBox.getPosition().x, currViewTop);
    }
}

void TextItemList::setTextFont(std::string& fontFile)
{
    if (!textItemFont->loadFromFile(fontFile))
        exit(308);
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

        unsigned int i = 0;
        while(i < textItemList.size())
        {

            if(textItemList[i].getTextBounds().contains(mousePos) && event.mouseButton.button == sf::Mouse::Left)
            {
           //     std::cout << "Mouse x: " << mousePos.x << std::endl;
            //    std::cout << "Mouse y: " << mousePos.y << std::endl;

                //Unhighlight the last selection if there was one
                if(selectionMade)
                    textItemList[currentSelection].toggleHighlight();

                //Highlight the new selection
                currentSelection = i;
                textItemList[currentSelection].toggleHighlight();
                selectionMade = true;
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

    window.setView(itemListView);

    if(backgroundBoxVisible)
    {
        window.draw(viewableAreaBox);
        window.draw(scrollBar);
    }
    //Only draw text that is inside of the viewable area
    //To do: Associate each text item with a coordinate so we can jump straight to the top
    for (auto& textItem: textItemList)
    {
        //If the bottom of the view is greater than or equal to the bottom of the text item
        if(itemListView.getCenter().y + itemListView.getSize().y / 2 >= textItem.getTopPosition().y)
            window.draw(textItem);
    }

}

