// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <map>
#include <string>
#include <mutex>
#include <functional>
#include "network.h"
#include "clientmanager.h"
#include "miscnetwork.h"

class ServerNetwork
{
    typedef std::function<void(Client*)> logOutCallbackType;

    public:
        ServerNetwork(ClientManager&, PacketExtra&, PacketExtra&, unsigned short, logOutCallbackType);

        // These are called from the Server class in separate threads, and return after a packet
        // has been received and stored into the PacketExtra reference passed into the constructor
        void receiveUdpPacket();
        void receiveTcpPacket();

        // Send packets over the network
        // TCP
        void sendToAllTcp(sf::Packet&, ClientID exclude = -1, bool mustBeLoggedIn = true);
        void sendToClientTcp(sf::Packet&, ClientID, bool mustBeLoggedIn = true);
        void sendToClientTcp(sf::Packet&, const IpPort&, bool mustBeLoggedIn = true);
        // UDP
        void sendToAllUdp(sf::Packet&, ClientID exclude = -1, bool mustBeLoggedIn = true);
        void sendToClientUdp(sf::Packet&, ClientID, bool mustBeLoggedIn = true);
        void sendToClientUdp(sf::Packet&, const IpPort&, bool mustBeLoggedIn = true);
        void udpSend(Client*, sf::Packet&, bool mustBeLoggedIn = true);

        // Build and send packets
        void sendServerChatMessage(const std::string&, ClientID exclude = -1);

    private:
        void addClient();
        void removeClient(ClientID);
        void logOutClient(Client*);
        void testSockets();
        bool validatePacket(PacketExtra&, ClientID);

        sf::UdpSocket udpSock;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        ClientManager& clients;
        PacketExtra& udpPacket;
        PacketExtra& tcpPacket;
        bool receivedTcpPacket;
        logOutCallbackType logOutCallback;
};

#endif
