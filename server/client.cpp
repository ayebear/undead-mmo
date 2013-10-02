// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "client.h"

Client::Client(ClientID newID, sf::TcpSocket* tcpSockPtr)
    : id(newID),
    playerEid(0),
    tcpSock(tcpSockPtr),
    loggedIn(false)
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

void Client::logIn(EID entId)
{
    playerEid = entId;
    loggedIn = true;
}

void Client::logOut()
{
    loggedIn = false;
    playerEid = 0;
}

bool Client::isLoggedIn() const
{
    return loggedIn;
}
