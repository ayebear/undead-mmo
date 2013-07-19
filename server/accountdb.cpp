// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "accountdb.h"
#include "packet.h"
#include "configfile.h"

// These could actually be in the main server config file
const string AccountDb::accountDir = "accounts/";
const string AccountDb::accountListFilename = "accounts.txt";

AccountDb::AccountDb()
{
    loadAccountList();
}

AccountDb::AccountDb(const string& filename)
{
    loadAccountList(filename);
}

bool AccountDb::loadAccountList()
{
    return loadAccountList(accountListFilename);
}

bool AccountDb::loadAccountList(const string& filename)
{
    return accountList.loadAccountIndex(accountDir + filename);
}

int AccountDb::logIn(const string& username, const string& password, PlayerData& pData)
{
    int status = Packet::Login::UnknownFailure;
    int accountId = accountList.getAccountId(username); // Get the account ID from the username
    if (accountId > 0) // Make sure the account ID is valid
    {
        string accountFilename = accountIdToFilename(accountId);
        ConfigFile accountCfg;
        if (accountCfg.loadConfigFile(accountFilename)) // Load the account config file
        {
            if (accountCfg.getOption("password").asString() == password) // Check if the password is correct!
            {
                if (!accountCfg.getOption("banned").asBool()) // Check if the account is banned
                {
                    pData.loadFromConfig(accountCfg); // Load the data from the config file into the player data object
                    status = Packet::Login::Successful;
                }
                else
                    status = Packet::Login::AccountBanned;
            }
            else
                status = Packet::Login::InvalidPassword;
        }
    }
    else
        status = Packet::Login::InvalidUsername;
    return status;
}

int AccountDb::createAccount(const PlayerData& pData)
{
    int status = Packet::Login::UnknownFailure;
    int accountId = accountList.getAccountId(pData.username); // Get the account ID from the username
    // TODO: We could also check if the password is strong enough, but this should be user-configurable, like minimum length, different characters, etc.
    if (accountId == -1) // Don't try to create a new account if it already exists!
    {
        int newAccountId = accountList.addAccount(pData.username); // Add the username to the account list and get its new ID
        if (newAccountId > 0) // If the account was added to the list successfully
        {
            string accountFilename = accountIdToFilename(newAccountId);
            ConfigFile accountCfg;
            pData.saveToConfig(accountCfg); // Set the values in the config file in memory from the player data object

            if (accountCfg.writeConfigFile(accountFilename)) // Write the account to a file
                status = Packet::Login::Successful;
        }
    }
    else
        status = Packet::Login::InvalidUsername;
    return status;
}

bool AccountDb::saveAccount(const PlayerData& pData)
{
    bool status = false;
    int accountId = accountList.getAccountId(pData.username);
    if (accountId > 0)
    {
        string accountFilename = accountIdToFilename(accountId);
        ConfigFile accountCfg;
        pData.saveToConfig(accountCfg); // Save the player data to a config file in memory
        status = accountCfg.writeConfigFile(accountFilename); // Write the config file in memory to the file
    }
    return status;
}

string AccountDb::accountIdToFilename(int id)
{
    return accountDir + toString<int>(id) + ".txt";
}
