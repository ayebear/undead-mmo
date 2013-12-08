// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "accountindex.h"
#include <sstream>
#include <fstream>
#include "option.h"

using namespace std;

IntString::IntString(): num(0) {}

IntString& IntString::operator=(const string& data)
{
    // Always set the string, but set the number to 0 if it fails
    str = data;
    istringstream tmp(data);
    if (!(tmp >> num))
        num = 0;
    /*
    // If it fails then change nothing
    istringstream tmp(data);
    if (tmp >> num)
        str = data;
    */
    return *this;
}

AccountIndex::AccountIndex()
{
    numOfAccounts = 0;
}

bool AccountIndex::loadAccountIndex(const string& filename)
{
    //cout << "loadAccountIndex()...\n";
    indexFilename = filename;
    bool status = false;
    ifstream file(filename, ifstream::in); // Open the file
    if (file.is_open())
    {
        bool good = true;
        string username, accountId;
        IntString maxNum;
        while (good && getline(file, username)) // Read a line
        {
            if (getline(file, accountId)) // Read a 2nd line
            {
                if (!username.empty() && !accountId.empty()) // Ignore any empty lines
                {
                    //cout << "Username: " << username << ", ID: " << accountId << endl;
                    index[username] = accountId; // Store the account ID in memory
                    maxNum = accountId;
                    if (maxNum.num > numOfAccounts) // If this account ID is greater than the current maximum ID
                        numOfAccounts = maxNum.num; // Then save this as the number of accounts
                    // By doing this, it supports out-of-order account indexes, rather than just storing the last account ID
                }
            }
            else
                good = false;
        }
        status = true;
    }
    return status;
}

// This adds an account to the list, and automatically assigns an account ID.
// If it failed, or if the username already exists, it returns -1
int AccountIndex::addAccount(const string& username)
{
    int id = -1;
    if (getAccountId(username) == -1) // Only add the account if it does not already exist
    {
        ofstream outFile(indexFilename, ofstream::out | ofstream::app);
        if (outFile.is_open())
        {
            numOfAccounts++; // Generate a new account ID
            string idStr = to_string(numOfAccounts);
            index[username] = idStr; // Store the new account in memory
            outFile << username + '\n' + idStr + '\n'; // Append the username and ID to the file
            outFile.close();
            id = numOfAccounts; // Set the return value to the new ID
        }
    }
    return id;
}

// Returns the account ID of the username passed in
// If it does not exist, then it returns -1
int AccountIndex::getAccountId(const string& username)
{
    int id = -1;
    auto found = index.find(username);
    if (found != index.end())
        id = found->second.num;
    return id;
}
