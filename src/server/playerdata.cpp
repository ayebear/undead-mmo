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

PlayerData::PlayerData(cfg::File& config)
{
    loadFromConfig(config);
}

void PlayerData::loadFromConfig(cfg::File& config)
{
    passwordHash = config("password").toString();
    salt = config("salt").toInt();
    health = config("health").toInt();
    level = config("level").toInt();
    banned = config("banned").toBool();
    positionX = config("positionX").toDouble();
    positionY = config("positionY").toDouble();
    inventory.loadFromConfig(config);
}

void PlayerData::saveToConfig(cfg::File& config) const
{
    config("password") = passwordHash;
    config("salt") = salt;
    config("health") = health;
    config("level") = level;
    config("banned") = banned;
    config("positionX") = positionX;
    config("positionY") = positionY;
    inventory.saveToConfig(config);
}
