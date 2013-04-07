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
        AddMessage(string);
    private:
        deque <string> msgList;
};

#endif // CHAT_H
