// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "playerdata.h"

PlayerData::PlayerData()
    : salt(0),
    banned(false),
    health(0),
    level(0),
    positionX(100),
    positionY(100)
{
}

PlayerData::PlayerData(ConfigFile& cfg)
{
    loadFromConfig(cfg);
}

void PlayerData::loadFromConfig(ConfigFile& cfg)
{
    passwordHash = cfg["password"].asString();
    salt = cfg["salt"].asInt();
    health = cfg["health"].asInt();
    level = cfg["level"].asInt();
    banned = cfg["banned"].asBool();
    positionX = cfg["positionX"].asDouble();
    positionY = cfg["positionY"].asDouble();
    inventory.loadFromConfig(cfg);
}

void PlayerData::saveToConfig(ConfigFile& cfg) const
{
    cfg["password"].setString(passwordHash);
    cfg["salt"].set(salt);
    cfg["health"].set(health);
    cfg["level"].set(level);
    cfg["banned"].set(banned);
    cfg["positionX"].set(positionX);
    cfg["positionY"].set(positionY);
    inventory.saveToConfig(cfg);
}
