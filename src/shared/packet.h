// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKET_H
#define PACKET_H

#include <string>

namespace Packet
{
    // This is sent with the login packet
    const int ProtocolVersion = 8;

    // This type is sent with every packet so the code that receives it can determine how to process it
    // Please refer to the documentation for more information about these types
    enum Type
    {
        // Client/server needs to receive these
        LogInStatus = 1, // Sent from the server to tell the client if the log in was successful
        CreateAccountStatus, // Sent from the server to tell the client if their account was created successfully
        ChatMessage, // Includes private/public/server messages
        EntityUpdate, // New/deleted/updated entities
        MapData, // All of the logical tiles for the map; is automatically sent from the server on successful login
        InventoryUpdate, // Updates slot(s) in the inventory
            // Note: The first value is the size of the inventory
        OnSuccessfulLogIn, // Data sent after successfully logging in
        MultiPacket,

        PacketTypes, // For the client

        // Server only needs to receive these
        Input,
        LogIn,
        LogOut,
        CreateAccount,
        GetPlayerList,
        GetServerInfo,

        TotalPacketTypes // For the server
    };

    // Sub-types
    namespace LogInCode
    {
        enum Type
        {
            // Can be returned by server
            Successful = 1,
            InvalidUsername,
            InvalidPassword,
            AccountBanned,
            AlreadyLoggedIn,
            ProtocolVersionMismatch,
            OtherServerError,
            // Cannot be returned by server
            Timeout,
            ErrorConnecting,
            UnknownFailure
        };
    }
    namespace CreateAccountCode
    {
        enum Type
        {
            // Can be returned by server
            Successful = 1,
            UsernameExists,
            WeakPassword,
            ProtocolVersionMismatch,
            OtherServerError,
            // Cannot be returned by server
            Timeout,
            ErrorConnecting,
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
    namespace InputType
    {
        enum Type
        {
            StartMoving = 1,
            StopMoving,
            ChangeVisualAngle,
            UseItem,
            PickupItem,
            DropItem,
            SwapItem,
            WieldItem
        };
    }
    const std::string LogInMessages[] = {
        "Successfully logged in.",
        "Invalid username.",
        "Invalid password.",
        "Account has been banned.",
        "Account is already logged in.",
        "Protocol version mismatch.",
        "Other server error.",
        "Response timed out.",
        "Error connecting to server.",
        "Unknown failure."
    };
    const std::string CreateAccountMessages[] = {
        "Successfully created account.",
        "Username already exists.",
        "Password does not meet requirements of server.",
        "Protocol version mismatch.",
        "Other server error.",
        "Response timed out.",
        "Error connecting to server.",
        "Unknown failure."
    };
}

#endif
