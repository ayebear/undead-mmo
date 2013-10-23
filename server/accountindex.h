// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ACCOUNTINDEX_H
#define ACCOUNTINDEX_H

#include <string>
#include <map>

using namespace std;

// A lightweight "Option"-like class
class IntString
{
    public:
        IntString();
        IntString& operator=(const string&);

        int num;
        string str;
};

// This class manages a list of account usernames, and was made only for performance improvements because of ConfigFile's slow file writing
class AccountIndex
{
    public:
        AccountIndex();
        bool loadAccountIndex(const string&); // filename (accounts.txt)

        int addAccount(const string&); // takes username, returns account ID
        int getAccountId(const string&); // takes username, returns account ID

    private:
        string indexFilename;
        int numOfAccounts;
        map<string,IntString> index;
};

#endif
