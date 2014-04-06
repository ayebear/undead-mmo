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

PlayerData::PlayerData(cfg::File& cfg)
{
    loadFromConfig(cfg);
}

void PlayerData::loadFromConfig(cfg::File& cfg)
{
    passwordHash = cfg("password").toString();
    salt = cfg("salt").toInt();
    health = cfg("health").toInt();
    level = cfg("level").toInt();
    banned = cfg("banned").toBool();
    positionX = cfg("positionX").toDouble();
    positionY = cfg("positionY").toDouble();
    inventory.loadFromConfig(cfg);
}

void PlayerData::saveToConfig(cfg::File& cfg) const
{
    cfg("password") = passwordHash;
    cfg("salt") = salt;
    cfg("health") = health;
    cfg("level") = level;
    cfg("banned") = banned;
    cfg("positionX") = positionX;
    cfg("positionY") = positionY;
    inventory.saveToConfig(cfg);
}
