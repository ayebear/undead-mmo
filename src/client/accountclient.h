#ifndef ACCOUNTCLIENT_H
#define ACCOUNTCLIENT_H

#include <string>
#include "client.h"

/*
This class can handle log-ins, creating accounts, etc.
*/
class AccountClient
{
    public:
        AccountClient(net::Client& client);

        int logIn(const net::Address& address, const std::string& username, const std::string& password);
        int createAccount(const net::Address& address, const std::string& username, const std::string& password);
        void logOut();
        const std::string& getUsername();
        const std::string getStatusString();

    private:
        net::Client& client;
        std::string currentUsername;
        bool loggedIn;
        int status;
        bool packetReceived;
};

#endif
