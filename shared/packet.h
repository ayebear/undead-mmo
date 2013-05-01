// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKET_H
#define PACKET_H

namespace Packet
{
    // This type is sent with every packet so the code that receives it can determine how to process it
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
};

#endif // PACKET_H
