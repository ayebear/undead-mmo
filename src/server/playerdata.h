// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <string>
#include "configfile.h"
#include "inventory.h"

/*
This class stores all of the information associated about a player's actual character.
This includes all private information that only the server and a single client should know about,
    and excludes everything in the Player entity class.
This is used in the account database, for loading and saving accounts.
This is also used by the client to store its player data.
The player's player entity (location, health, etc.) should be stored in here as well!
The inventory and current stats should be stored here.
*/
class PlayerData
{
    public:
        PlayerData();
        PlayerData(cfg::File&);
        void loadFromConfig(cfg::File&);
        void saveToConfig(cfg::File&) const;

        std::string username;
        std::string passwordHash;
        unsigned int salt;
        bool banned;
        int health;
        int level;
        float positionX, positionY; // TODO: Store the entire entity so that the wielded items and stuff are stored
        Inventory inventory; // The player's inventory

    private:
};

#endif
