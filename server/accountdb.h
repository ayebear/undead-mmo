#ifndef ACCOUNTDB_H
#define ACCOUNTDB_H

#include "configfile.h"
#include "playerdata.h"

class AccountDb
{
    public:
        AccountDb(); // Uses default account list filename
        AccountDb(const string&); // Takes filename of account list
        bool loadAccountList(); // Same as constructor
        bool loadAccountList(const string&); // Same as constructor

        bool login(const string&, const string&, PlayerData&); // username, password, player data object to load into
        bool loadAccount(const string&, PlayerData&); // username, player data object to load into
        bool saveAccount(const string&, const PlayerData&); // username, player data object to read from

    private:
        ConfigFile accountList; // Stores a list of usernames and account IDs like this: username = accountID

};

#endif
