#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <string>

using namespace std;

/*
This class stores all of the information associated about a player's actual character.
This includes all private information that only the server and 1 client should know about,
    and excludes everything in the Player entity class.
This is used in the account database, for loading and saving accounts.
This is also used by the client to store its player data.
*/
class PlayerData
{
    public:
        PlayerData();
        string username;
        string passwordHash;
        unsigned int salt;
    private:
};

#endif
