#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <map>
#include "tcpserver.h"
#include "address.h"
#include "playerdata.h"
#include "entity.h"

/*
This class contains all of the temporary things needed for when a client is logged in.
*/
struct Player
{
    Player();
    Player(int id);
    Player(int id, const net::Address& address, EID playerEid);

    int id; // The ID of the client from the TCP server
    net::Address address; // IP address and port

    EID playerEid; // The entity ID of the player's entity
    PlayerData playerData; // The player's game data
};

/*
This class manages players logging in/out.
    It contains a map of Player objects.
Players added to this class are represented as being logged in.
When they are removed, they are logged out.
*/
class PlayerManager
{
    using PlayerMap = std::map<int, Player>;

    public:
        PlayerManager(net::TcpServer& tcpServer);
        ~PlayerManager();
        Player& addPlayer(int id);
        Player* getPlayer(int id);
        Player* getPlayer(const std::string& username);
        void removePlayer(int id);
        void send(sf::Packet& packet);

    private:
        net::TcpServer& tcpServer;
        PlayerMap players;
};

#endif
