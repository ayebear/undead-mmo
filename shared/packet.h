// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKET_H
#define PACKET_H

// this class will store useful information, by extracting it from a string
// could also do the reverse maybe.
// this could be used for having different packet "types" that all mean different things, like a character coordinate update, or new zombies or items

// we may not need this class depending on how the server will handle packets
// but we will definitely need the enum

class Packet
{
    public:
        Packet();
        enum Type
        {
            // Updates (client to server)
            LogIn = 1,
            LogOut,
            ChatMessage,
            PlayerUpdate, // may need to be more specific with this
            // Updates (server to client(s))
            //ChatMessageToClients,
            NewEntity,
            EntityUpdate,
            // Requests (client to server)
            GetMap,
            GetEntityList,
            GetPlayerList,
            //GetChatHistory,
            // Requests (server to client)
            Ping
        };
    private:
};

#endif // PACKET_H
