#include "playermanager.h"

PlayerManager::PlayerManager(net::TcpServer& tcpServer):
    tcpServer(tcpServer)
{
}

PlayerManager::~PlayerManager()
{
}

Player::Player():
    id(-1),
    playerEid(-1)
{
}

Player::Player(int id):
    id(id),
    playerEid(-1)
{
}

Player::Player(int id, const net::Address& address, EID playerEid):
    id(id),
    address(address),
    playerEid(playerEid)
{
}

Player& PlayerManager::addPlayer(int id)
{
    //players.emplace(id, id);//, tcpServer.getClientAddress(id), 0);
    auto& player = players[id];
    player.id = id;
    player.address.ip = tcpServer.getClientAddress(id);
    return player;
}

Player* PlayerManager::getPlayer(int id)
{
    auto found = players.find(id);
    return (found != players.end() ? &found->second : nullptr);
}

Player* PlayerManager::getPlayer(const std::string& username)
{
    // TODO: Use another map for better performance
    for (auto& player: players)
    {
        if (player.second.playerData.username == username)
            return &player.second;
    }
    return nullptr;
}

void PlayerManager::removePlayer(int id)
{
    players.erase(id);
}

void PlayerManager::send(sf::Packet& packet)
{
    for (auto& player: players)
        tcpServer.send(packet, player.first);
}
