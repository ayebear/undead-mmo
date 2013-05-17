// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKET_H
#define PACKET_H

namespace Packet
{
    // This type is sent with every packet so the code that receives it can determine how to process it
    enum Type
    {
        // Packets which the client will need to receive
        AuthStatus = 1, // Sent from the server to tell the client if the login was successful
        Input, // Keyboard/mouse input from client to server
        ChatMessage, // Includes private/public/server messages
        EntityUpdate, // Either a new entity or just updated data
        GetPlayerList, // Get list of currently logged in players
        //PacketTypes, // For the client - see below

        // Server specific packets that the client won't need to store
        LogIn,
        LogOut,
        CreateAccount,

        //TotalPacketTypes // For the server - see below
        PacketTypes, // This saves us a lot of issues, and is not a big deal if we store a few empty linked lists...
    };

    // Sub-types
    namespace Auth
    {
        enum Type
        {
            Successful = 1,
            InvalidUsername,
            InvalidPassword,
            AccountBanned,
            UnknownFailure
        };
    }
    namespace Chat
    {
        enum Type
        {
            Public = 1,
            Server,
            Private
        };
    }
}

#endif
