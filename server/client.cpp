#include "client.h"

Client::Client(ClientID newID, sf::TcpSocket* tcpSockPtr)
    : id(newID),
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
