#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>

using namespace std;

// This class handles the client-side stuff for the chat like graphics and input

class Chat
{
    public:
        Chat();
        void Show();
        void Hide();
        void AddMessage(string);
    private:
        bool shown;
        deque <string> msgList;
};

#endif // CHAT_H
