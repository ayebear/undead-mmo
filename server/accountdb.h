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

        bool loadAccount(PlayerData&);
        bool saveAccount(const PlayerData&);

    private:
        ConfigFile accountList;

};

#endif
