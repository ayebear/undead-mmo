// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;

struct TimedMsg
{
    TimedMsg(sf::Text& t): text(t) {}
    sf::Text text;
    sf::Clock age;
};

// This class handles the client-side stuff for the chat like graphics and input

class Chat: public sf::Drawable
{
    public:
        Chat();
        void SetInput(bool);
        bool GetInput();
        void ToggleInput();
        void SetPosition(float, float);
        void FixCursorPosition();
        void FixPositions();
        void AddChar(char);
        void RemoveChar();
        void AddMessage(sf::Text);
        void AddMessage(string);
        void SendMessage(sf::TcpSocket&);
        void Update();
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        static const ushort maxMessages;
        static const short textSize;
        static const float cursorBlinkRate;
        static const float oldMsgAge;
        static const float maxMsgAge;
        bool input;
        bool showCursor;
        sf::Vector2f mainPos;
        sf::Font font;
        deque <TimedMsg> msgList;
        sf::Text currentMsg;
        sf::RectangleShape cursor;
        sf::Clock cursorTimer;
};

#endif // CHAT_H
