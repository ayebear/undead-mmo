#include "client.h"

Client::Client(ClientID newID, sf::TcpSocket* tcpSockPtr)
    : id(newID),
    tcpSock(tcpSockPtr),
    loggedIn(false),
    playerEid(0)
{
    address.ip = tcpSockPtr->getRemoteAddress();
    address.port = tcpSockPtr->getRemotePort();
}

Client::~Client()
{
    tcpSock.reset();
}

void Client::tcpSend(sf::Packet& packet, bool mustBeLoggedIn)
{
    // The client must either be logged in or not required to be logged in to send the packet.
    if (loggedIn || !mustBeLoggedIn)
        tcpSock->send(packet);
}

void Client::logIn(const std::string& usernameStr, EID entId)
{
    username = usernameStr;
    playerEid = entId;
    loggedIn = true;
}

void Client::logOut()
{
    loggedIn = false;
    username.clear();
    playerEid = 0;
}
