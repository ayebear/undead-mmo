#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <string>
#include "../shared/network.h"

class ClientNetwork: public Network
{
    public:
        ClientNetwork();
        void ReceiveTcp();
        bool ConnectToServer(const sf::IpAddress&);
        int Login(const std::string&, const std::string&);
        void SendChatMessage(const std::string&);
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);
        bool IsConnected();
    private:
        // Client only needs a single TCP socket because it is only communicating with the server
        sf::TcpSocket tcpSock;
        sf::IpAddress serverAddress;
        bool connected;
};

#endif
