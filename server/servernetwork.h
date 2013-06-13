#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <map>
#include <string>
#include "../shared/network.h"
#include "../shared/linkedqueue.h"

typedef unsigned int ClientID;

struct PacketExtra
{
    sf::Packet data;
    ClientID sender;
};

struct Client
{
    Client(): tcpSock(nullptr), port(Network::defaultPort), loggedIn(false), id(idCounter++) {}
    Client(sf::TcpSocket* tcpSockPtr);
    ~Client();
    sf::TcpSocket* tcpSock;
    // These are required for UDP
    sf::IpAddress ip;
    unsigned short port;
    std::string username;
    bool loggedIn; // They can be connected but not logged in
    ClientID id; // This will also be used for the key in the map

    static ClientID idCounter;
};

class ServerNetwork: public Network
{
    public:
        ServerNetwork();

        // Overidden functions
        void ReceiveUdp();
        void ReceiveTcp();

        // Packet functions
        bool ArePackets();
        PacketExtra& GetPacket();
        void PopPacket();
        void StorePacket(sf::Packet&, ClientID);
        void StorePacket(sf::Packet&, sf::IpAddress&);

        // Other functions
        const std::string GetStatusString();
        bool ValidAddress(sf::IpAddress);
        bool ValidClientID(ClientID);
        void SendToAll(sf::Packet&, ClientID exclude = -1);
        void SendToClient(sf::Packet&, ClientID);
        void PrintClients();
        void AddClient();
        void RemoveClient(sf::TcpSocket&, ClientID);
        void TestSockets();
        bool IsValidType(int);

    private:
        LinkedQueue<PacketExtra> packets; // Stores all received packets
        std::map<ClientID, Client> clients; // Stores the clients
        sf::TcpListener listener;
        sf::SocketSelector selector;
};

#endif
