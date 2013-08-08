// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <string>
#include "linkedqueue.h"
#include "network.h"
#include "packet.h"
#include "ipport.h"

/*
This class handles all of the client side networking.
It organizes received packets by their type into different lists inside of an array.
This way things can more easily read specific packets received instead of everything filtering through all of them.
*/
class ClientNetwork: public Network
{
    public:
        ClientNetwork();
        ~ClientNetwork();

        // Overridden functions
        void receiveUdp();
        void receiveTcp();

        // Packet storage functions
        bool arePackets(int); // determines if there are any packets in the lists
        sf::Packet& getPacket(int);
        void popPacket(int);
        void clearPackets(int);
        void clearAllPackets();
        void storePacket(sf::Packet&);

        // Packet helpers
        void sendPacketTcp(sf::Packet&);
        void sendPacketUdp(sf::Packet&);
        void sendChatMessage(const std::string&);
        void sendChatMessage(const std::string&, const std::string&);

        // Server control
        void setServerAddress(const sf::IpAddress&); // Sets the server address
        int logIn(const sf::IpAddress&, const std::string&, const std::string&); // This sets the server address as well as logging in
        int logIn(const std::string&, const std::string&); // Logs into the currently connected server
        int createAccount(const sf::IpAddress&, const std::string&, const std::string&); // This also sets the server address
        int createAccount(const std::string&, const std::string&); // Requests a server to create a new account on it
        void logOut();
        const std::string& getUsername();
        const std::string getStatusString();
        bool validAddress(sf::IpAddress);
        bool isConnected();
        bool isValidType(int);

    private:
        bool connectToServer();
        void disconnectFromServer();

        // Client only needs a single TCP socket because it is only communicating with the server
        sf::TcpSocket tcpSock;
        IpPort serverAddress;
        bool connected;

        std::string currentUsername;

        // All received packets will be stored in here. Only valid packets sent from the server will be stored.
        LinkedQueue<sf::Packet> packets[Packet::PacketTypes];
};

#endif
