#include <iostream>
#include <string>
#include "accountdb.h"
#include "playerdata.h"
#include "packet.h"

using namespace std;

string randomString(int length);
void loginTest(AccountDb&);
void generateAccounts(AccountDb&);
void createAccountTest(AccountDb&);

int main()
{
    AccountDb testDb;
    //generateAccounts(testDb);
    createAccountTest(testDb);
    loginTest(testDb);
    return 0;
}

void loginTest(AccountDb& db)
{
    PlayerData test;
    string username, password;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    int status = db.logIn(username, password, test);
    if (status == Packet::Login::Successful)
    {
        cout << "Successfully logged in!\n";
        cout << "Level: " << test.level << ", Health: " << test.health << endl;
        cout << "Enter new level: ";
        cin >> test.level;
        if (db.saveAccount(test))
            cout << "Successfully saved account data!\n";
        else
            cout << "Error: Could not save account data.\n";
    }
    else
        cout << "Error: Could not log in. Status code = " << status << endl;
}

void generateAccounts(AccountDb& db)
{
    PlayerData test;
    test.health = 100;
    test.level = 5;
    test.username = "test";
    test.passwordHash = "password";
    db.createAccount(test);

    /*srand(time(nullptr));
    cout << "Creating accounts...\n";
    for (int x = 0; x < 1000; x++)
    {
        test.health = rand() % 101;
        test.level = rand() % 50;
        test.username = randomString(rand() % 24 + 8);
        test.passwordHash = randomString(rand() % 40 + 10);
        test.salt = rand() % 1000000;
        test.banned = ((rand() % 100) == 50);
        int status = db.createAccount(test);
        if (status != Packet::Login::Successful)
            cout << status << endl;
    }
    cout << "Done!\n";*/
}

void createAccountTest(AccountDb& db)
{
    PlayerData test;
    cout << "Create a new account:\n";
    cout << "Enter username: ";
    getline(cin, test.username);
    cout << "Enter password: ";
    getline(cin, test.passwordHash);
    int status = db.createAccount(test);
    if (status == Packet::Login::Successful)
        cout << "Successfully created new account.\n";
    else
        cout << "Error: Could not create account. Status code = " << status << endl;
}

string randomString(int length)
{
    string str;
    char c;
    for (int x = 0; x < length; x++)
    {
        int type = rand() % 3;
        if (type == 0)
            c = 'A' + rand() % 26;
        else if (type == 1)
            c = 'a' + rand() % 26;
        else if (type == 2)
            c = '0' + rand() % 10;

        str += c;
    }
    return str;
}

