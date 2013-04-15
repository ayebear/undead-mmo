#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>

using namespace std;

// This class handles the client-side stuff for the chat like graphics and input

class Chat
{
    public:
        Chat();
        void SetInput(bool);
        bool GetInput();
        void SetPosition(float, float);
        void FixPositions();
        void AddChar(char);
        void RemoveChar();
        void AddMessage(sf::Text);
        void AddMessage(string);
        void SendMessage();
        void Draw(sf::RenderWindow&);
    private:
        static const ushort maxMessages = 10;
        short textSize;
        bool input;
        sf::Vector2f mainPos;
        sf::Font font;
        deque <sf::Text> msgList;
        sf::Text currentMsg;
        // remember to add the cursor and text box and chat box and stuff
        // maybe we can make an image for that but still add the cursor
};

#endif // CHAT_H
