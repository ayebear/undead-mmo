#include "accountclient.h"
#include <iostream>
#include "packet.h"

AccountClient::AccountClient(net::Client& client):
    client(client)
{
    loggedIn = false;
    packetReceived = false;
    auto accountPacketHandler = [&](sf::Packet& packet)
    {
        packet >> status;
        packetReceived = true;
    };
    client.registerCallback(Packet::LogInStatus, accountPacketHandler);
    client.registerCallback(Packet::CreateAccountStatus, accountPacketHandler);

    // Not using these yet, because it will drop the other packets we need after successfully logging in
    //client.setGroup("login", {Packet::LogInStatus});
    //client.setGroup("createAccount", {Packet::CreateAccountStatus});
}

int AccountClient::logIn(const net::Address& address, const std::string& username, const std::string& password)
{
    currentUsername = username;
    status = Packet::LogInCode::ErrorConnecting;
    const float timeout = 5.0f;
    std::cout << "Connecting to " << address.toString() << "... ";
    // Attempt to connect to the server
    if (client.connect(address, sf::seconds(timeout)))
    {
        std::cout << "Done!\nLogging in...\n";
        status = Packet::LogInCode::UnknownFailure;

        // Send login packet
        sf::Packet loginPacket;
        loginPacket << Packet::LogIn << Packet::ProtocolVersion << username << password;
        client.send(loginPacket);

        // Wait until you get a response from the server for your log in request
        // TODO: Move this outside of this function and have a function for polling if it was successful.
        packetReceived = false;
        sf::Clock loginTimer;
        while (!packetReceived && !client.receive() && loginTimer.getElapsedTime().asSeconds() < timeout)
            sf::sleep(sf::milliseconds(10));

        if (!packetReceived)
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
    status = Packet::CreateAccountCode::ErrorConnecting;
    const float timeout = 5.0f;
    std::cout << "Connecting to " << address.toString() << "... ";
    // Attempt to connect to the server
    if (client.connect(address, sf::seconds(timeout)))
    {
        std::cout << "Done!\nCreating account...\n";
        status = Packet::CreateAccountCode::UnknownFailure;

        // Send create account packet
        sf::Packet createAccountPacket;
        createAccountPacket << Packet::CreateAccount << Packet::ProtocolVersion << username << password;
        client.send(createAccountPacket);

        // Wait until you get a response from the server for your create account request
        // TODO: Move this outside of this function and have a function for polling if it was successful.
        packetReceived = false;
        sf::Clock loginTimer;
        while (!packetReceived && !client.receive() && loginTimer.getElapsedTime().asSeconds() < timeout)
            sf::sleep(sf::milliseconds(10));

        if (!packetReceived)
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
