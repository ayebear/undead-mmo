// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "accountdb.h"
#include "packet.h"
#include "configfile.h"

const std::string AccountDb::accountListFilename = "accounts.txt";

AccountDb::AccountDb()
{
    loadAccountList("accounts/");
}

AccountDb::AccountDb(const std::string& dir)
{
    loadAccountList(dir);
}

bool AccountDb::loadAccountList(const std::string& dir)
{
    accountDir = dir;
    strlib::mustEndWith(accountDir, "/");
    return accountList.loadAccountIndex(accountDir + accountListFilename);
}

int AccountDb::logIn(const std::string& username, const std::string& password, PlayerData& playerData)
{
    int status = Packet::LogInCode::UnknownFailure;
    int accountId = accountList.getAccountId(username); // Get the account ID from the username
    if (accountId > 0) // Make sure the account ID is valid
    {
        std::string accountFilename = accountIdToFilename(accountId);
        cfg::File accountCfg;
        if (accountCfg.loadFromFile(accountFilename)) // Load the account config file
        {
            if (accountCfg("password").toString() == password) // Check if the password is correct!
            {
                if (!accountCfg("banned").toBool()) // Check if the account is banned
                {
                    playerData.loadFromConfig(accountCfg); // Load the data from the config file into the player data object
                    playerData.username = username; // Make sure we set the username!
                    status = Packet::LogInCode::Successful;
                }
                else
                    status = Packet::LogInCode::AccountBanned;
            }
            else
                status = Packet::LogInCode::InvalidPassword;
        }
    }
    else
        status = Packet::LogInCode::InvalidUsername;
    return status;
}

int AccountDb::createAccount(const PlayerData& playerData)
{
    int status = Packet::CreateAccountCode::UnknownFailure;
    int accountId = accountList.getAccountId(playerData.username); // Get the account ID from the username
    // TODO: We could also check if the password is strong enough, but this should be user-configurable, like minimum length, different characters, etc.
    if (accountId == -1) // Don't try to create a new account if it already exists!
    {
        int newAccountId = accountList.addAccount(playerData.username); // Add the username to the account list and get its new ID
        if (newAccountId > 0) // If the account was added to the list successfully
        {
            std::string accountFilename = accountIdToFilename(newAccountId);
            cfg::File accountCfg;
            playerData.saveToConfig(accountCfg); // Set the values in the config file in memory from the player data object

            if (accountCfg.writeToFile(accountFilename)) // Write the account to a file
                status = Packet::CreateAccountCode::Successful;
        }
    }
    else
        status = Packet::CreateAccountCode::UsernameExists;
    return status;
}

bool AccountDb::saveAccount(const PlayerData& playerData)
{
    bool status = false;
    int accountId = accountList.getAccountId(playerData.username);
    if (accountId > 0)
    {
        std::string accountFilename = accountIdToFilename(accountId);
        cfg::File accountCfg;
        playerData.saveToConfig(accountCfg); // Save the player data to a config file in memory
        status = accountCfg.writeToFile(accountFilename); // Write the config file in memory to the file
    }
    return status;
}

std::string AccountDb::accountIdToFilename(int id)
{
    return accountDir + std::to_string(id) + ".txt";
}
