#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <SFML/Graphics.hpp>
#include <string>
#include "cursor.h"

using namespace std;

// A class that handles processing text input and displaying it on the screen
class InputBox: public sf::Drawable
{
    public:
        InputBox();

        // Font size, Font, x Pos, y Pos, width, password mode
        void setUp(int, sf::Font&, float, float, float, bool);
        // render window, font size, font, width of box, x Pos, Y Pos,
        //void setUp(sf::RenderWindow)
        // Stuff you would mainly use in a loop
        void processInput(sf::Keyboard::Key); // In the future this can also take in mouse input (or make another function for that)
        void updateCursor();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

        // Settings
        void setPosition(float, float);
        void setPasswordMode(bool);
        void setShowRectangle(bool);
        void setTextSize(unsigned int);
        void setColor(sf::Color);
        void setString(const string& str);
        void setInput(bool);

        // Other
        void updateCursorPos();
        const string& getString();
        void clear();
        void resetCursor();
        void addChar(char);

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
        sf::Vector2f pos;
        sf::RectangleShape inputBorder;
        Cursor cursor;

        // Settings
        sf::Font* font;
        bool input;
        bool pwdMode;
        bool showBorder;
};

#endif
