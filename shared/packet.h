// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKET_H
#define PACKET_H

namespace Packet
{
    // This type is sent with every packet so the code that receives it can determine how to process it
    enum Type
    {
        LogIn = 1, // also gets map + entities + players
        Input, // keyboard/mouse input from client to server
        ChatMessage, // includes private/public/server messages
        EntityUpdate,
        GetPlayerList, // get list of currently logged in players
        PacketTypes
    };
};

#endif // PACKET_H
