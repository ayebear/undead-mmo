#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <SFML/Graphics.hpp>
#include <string>
#include "cursor.h"

using namespace std;
/*
    TO DO:

    Need to optimize the text updating code so it's not a sequential search. Was hoping for a way to find character index by position,
    but I haven't seen anything like that in SFML

    Need to re-implement text history
*/
// A class that handles processing text input and displaying it on the screen
class InputBox: public sf::Drawable
{
    public:
        InputBox();

        // Font size, Font, x Pos, y Pos, width, password mode
        void setUp(int, sf::Font&, float, float, float, bool);

        // Stuff you would mainly use in a loop
        void processInput(sf::Keyboard::Key); // In the future this can also take in mouse input (or make another function for that)
        void updateCursor();
        void draw(sf::RenderTarget&, sf::RenderStates) const;
        void handleMouseClicked(sf::Event&, sf::RenderWindow&);
		void processTextEntered(sf::Uint32);

        // Settings
        void setPosition(float, float);
        void setPasswordMode(bool);
        void setShowRectangle(bool);
        void setTextSize(unsigned int);
        void setColor(sf::Color);
        void setString(const string& str);
        void setInput(bool);
        bool isActive();

        // Other
        void updateCursorPos();
        const string& getString();
        void clear();
        void resetCursor();
        void addChar(char);

        //Move the viewable characters bo left and right
        void shiftViewCharsRight();
        void shiftViewCharsLeft();

        //Used to find beginning and end of viewable characters
        void findLeftChar();
        void findRightChar();

    private:
        static const char pwdChar;

        // Text manipulation
        void backspace();
        void deleteChar();
        void updateText();

        // Cursor stuff
        void left();
        void right();
        void home();
        void end();

        // Variables
        string textStr;
        string pwdStr;
        sf::Text text;
        sf::Text viewableText;
        sf::Vector2f pos;
        sf::RectangleShape inputBorder;
        sf::FloatRect collisionBox;
        sf::FloatRect viewableChars;

        //Cursor that is visible
        Cursor cursor;

        //Cursor thst works behind the scenes
        Cursor invisibleCursor;
        unsigned int leftCharIndex;
        unsigned int rightCharIndex;

        //Used to find the position of the first and last characters using the width of the sub string before them
        sf::Text charsBeforeLeft;
        sf::Text charsBeforeRight;

        // Settings
        sf::Font* font;
        unsigned int fontSize;
        bool input;
        bool pwdMode;
        bool showBorder;
};

#endif
