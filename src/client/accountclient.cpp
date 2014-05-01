#include "accountclient.h"
#include <iostream>
#include "packet.h"

AccountClient::AccountClient(net::Client& client):
    client(client)
{
    loggedIn = false;
}

int AccountClient::logIn(const net::Address& address, const std::string& username, const std::string& password)
{
    client.clear();
    currentUsername = username;
    int status = Packet::LogInCode::ErrorConnecting;
    const float timeout = 5.0f;
    std::cout << "Connecting to " << address.toString() << "... ";
    // Attempt to connect to the server
    if (client.connect(address, sf::seconds(timeout)))
    {
        std::cout << "Done!\nLogging in...\n";
        status = Packet::LogInCode::UnknownFailure;
        client.clear(Packet::LogInStatus);

        // Send login packet
        sf::Packet loginPacket;
        loginPacket << Packet::LogIn << Packet::ProtocolVersion << username << password;
        client.tcpSend(loginPacket);

        // Wait until you get a response from the server for your log in request
        // TODO: Move this outside of this function and have a function for polling if it was successful.
        sf::Clock loginTimer;
        while (!client.arePackets(Packet::LogInStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            client.receive();
            sf::sleep(sf::milliseconds(10));
        }

        if (client.arePackets(Packet::LogInStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            client.getPacket(Packet::LogInStatus) >> status;
            client.popPacket(Packet::LogInStatus);
        }
        else
        {
            status = Packet::LogInCode::Timeout;
            std::cout << "Log in timed out.\n";
        }
    }
    else
        std::cout << "Error.\n";
    std::cout << "Login status: " << status << std::endl;
    // Prevent possibility of client receiving invalid status code from server
    if (status < 1 || status > Packet::LogInCode::UnknownFailure)
        status = Packet::LogInCode::UnknownFailure;
    return status;
}

int AccountClient::createAccount(const net::Address& address, const std::string& username, const std::string& password)
{
    client.clear();
    int status = Packet::CreateAccountCode::ErrorConnecting;
    const float timeout = 5.0f;
    std::cout << "Connecting to " << address.toString() << "... ";
    // Attempt to connect to the server
    if (client.connect(address, sf::seconds(timeout)))
    {
        std::cout << "Done!\nCreating account...\n";
        status = Packet::CreateAccountCode::UnknownFailure;
        client.clear(Packet::CreateAccountStatus);

        // Send create account packet
        sf::Packet createAccountPacket;
        createAccountPacket << Packet::CreateAccount << Packet::ProtocolVersion << username << password;
        client.tcpSend(createAccountPacket);

        // Wait until you get a response from the server for your create account request
        // TODO: Move this outside of this function and have a function for polling if it was successful.
        sf::Clock loginTimer;
        while (!client.arePackets(Packet::CreateAccountStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            client.receive();
            sf::sleep(sf::milliseconds(10));
        }

        if (client.arePackets(Packet::CreateAccountStatus) && loginTimer.getElapsedTime().asSeconds() < timeout)
        {
            client.getPacket(Packet::CreateAccountStatus) >> status;
            client.popPacket(Packet::CreateAccountStatus);
        }
        else
        {
            status = Packet::CreateAccountCode::Timeout;
            std::cout << "Create account timed out.\n";
        }

        // Disconnect from the server after creating the account
        client.disconnect();
    }
    else
        std::cout << "Error.\n";
    std::cout << "Create account status: " << status << std::endl;
    // Prevent possibility of client receiving invalid status code from server
    if (status < 1 || status > Packet::CreateAccountCode::UnknownFailure)
        status = Packet::CreateAccountCode::UnknownFailure;
    return status;
}

void AccountClient::logOut()
{
    std::cout << "Logging out...";
    client.disconnect();
    client.clear();
    std::cout << " Done.\n";
}

const std::string& AccountClient::getUsername()
{
    return currentUsername;
}

const std::string AccountClient::getStatusString()
{
    std::string status;
    if (!client.isConnected())
        status = "Currently not connected to any server.";
    else
        status = "Currently connected to a server.";
    return status;
}
