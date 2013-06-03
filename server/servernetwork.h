#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <deque>
#include "../shared/network.h"
#include "../shared/linkedqueue.h"

struct PacketExtra
{
    sf::Packet data;
    int sender;
};

struct Client
{
    Client(): tcpSock(nullptr), port(Network::defaultPort), loggedIn(false) {}
    Client(sf::TcpSocket* tcpSockPtr);
    sf::TcpSocket* tcpSock;
    // These are required for UDP
    sf::IpAddress ip;
    unsigned short port;
    string username;
    bool loggedIn; // They can be connected but not logged in
    // ID will already be known by position in deque
    // This seems to change so we cannot rely on this for the long term,
    // unless we have never changing UIDs and use a map instead of a deque
};

class ServerNetwork: public Network
{
    public:
        ServerNetwork();
        void ReceiveTcp();
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);

        // Packet functions
        bool ArePackets();
        sf::Packet& GetPacket();
        void PopPacket();
        void StorePacket(sf::Packet&);

        void SendToAll(sf::Packet&, int exclude = -1);
        void SendToClient(sf::Packet&, int);
        void PrintClients();
        void AddClient();
        void RemoveClient(sf::TcpSocket&, uint);
        void TestSockets();
        //void ProcessPacket(sf::Packet&, uint);
    private:
        LinkedQueue<PacketExtra> packets; // Stores all received packets
        std::deque<Client> clients; // Stores the clients
        sf::TcpListener listener;
        sf::SocketSelector selector;
};

#endif
