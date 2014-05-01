// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ACCOUNTINDEX_H
#define ACCOUNTINDEX_H

#include <string>
#include <map>

// A lightweight "Option"-like class
class IntString
{
    public:
        IntString();
        IntString& operator=(const std::string&);

        int num;
        std::string str;
};

// This class manages a list of account usernames, and was made only for performance improvements because of ConfigFile's slow file writing
class AccountIndex
{
    public:
        AccountIndex();
        bool loadAccountIndex(const std::string&); // filename (accounts.txt)

        int addAccount(const std::string&); // takes username, returns account ID
        int getAccountId(const std::string&); // takes username, returns account ID

    private:
        std::string indexFilename;
        int numOfAccounts;
        std::map<std::string,IntString> index;
};

#endif
