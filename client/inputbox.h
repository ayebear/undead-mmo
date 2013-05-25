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

        // Stuff you would mainly use in a loop
        void ProcessInput(sf::Keyboard::Key); // In the future this can also take in mouse input (or make another function for that)
        void UpdateCursor();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

        // Settings
        void SetFont(sf::Font*);
        void SetPosition(float, float);
        void SetPasswordMode(bool);
        void SetShowRectangle(bool);
        void SetTextSize(unsigned int);
        void SetColor(sf::Color);
        void SetString(const string& str);
        void SetInput(bool);

        // Other
        void UpdateCursorPos();
        const string& GetString();
        void Clear();
        void ResetCursor();
        void AddChar(char);

    private:
        static const char pwdChar;

        // Text manipulation
        void Backspace();
        void Delete();
        void UpdateText();

        // Cursor stuff
        void Left();
        void Right();
        void Home();
        void End();

        // Variables
        string textStr;
        string pwdStr;
        sf::Text text;
        sf::Vector2f pos;
        Cursor cursor;

        // Settings
        sf::Font* font;
        bool input;
        bool pwdMode;
        bool showRect;
};

#endif
