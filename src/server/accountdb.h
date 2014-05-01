// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ACCOUNTDB_H
#define ACCOUNTDB_H

#include "playerdata.h"
#include "accountindex.h"

/*
TODO:
    Automatically create the accounts directory if it doesn't already exist
*/

/*
This class will temporarily be used as the account database for the game server.
It relies on the ConfigFile class for storing each account in a separate file.
It relies on the AccountIndex class for storing a list of all of the accounts.
It probably isn't the best thing to use, but it will be good enough for now, and will simplify debugging as
    all of the accounts are just plain text files that can easily be edited.

The directory/file structure looks like this:
accounts/
    accounts.txt // Stores a list of usernames and account IDs
    1.txt // Named as account ID; starts at 1, not 0
    2.txt
    3.txt

Only accounts.txt is loaded into RAM, the rest is loaded/saved on demand.
As new accounts are created, the account ID increments each time.
    The account is also added to the accounts.txt file.
The main reason for using account IDs instead of just usernames is to allow more symbols in usernames.

Accounts.txt:
    test
    1
    someAccount
    2
    anotherAccount
    3
*/
class AccountDb
{
    public:
        AccountDb(); // Uses default account directory
        AccountDb(const std::string&); // Takes directory of accounts
        bool loadAccountList(const std::string&); // Same as constructor

        int logIn(const std::string&, const std::string&, PlayerData&); // Username, password, player data object to load into
        int createAccount(const PlayerData&); // Player data object to read from (username and password are stored in here)
        bool saveAccount(const PlayerData&); // Reads from the player data object and writes the account file

    private:
        std::string accountIdToFilename(int);

        std::string accountDir;
        static const std::string accountListFilename;

        AccountIndex accountList; // Stores a list of usernames and account IDs

};

#endif
