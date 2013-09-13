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
    username = cfg.getOption("username").asString();
    passwordHash = cfg.getOption("password").asString();
    salt = cfg.getOption("salt").asInt();
    health = cfg.getOption("health").asInt();
    level = cfg.getOption("level").asInt();
    banned = cfg.getOption("banned").asBool();
    positionX = cfg.getOption("positionX").asDouble();
    positionY = cfg.getOption("positionY").asDouble();
    inventory.loadFromConfig(cfg);
}

void PlayerData::saveToConfig(ConfigFile& cfg) const
{
    cfg.getOption("username").setString(username);
    cfg.getOption("password").setString(passwordHash);
    cfg.getOption("salt").set<int>(salt);
    cfg.getOption("health").set<int>(health);
    cfg.getOption("level").set<int>(level);
    cfg.getOption("banned").set<bool>(banned);
    cfg.getOption("positionX").set<double>(positionX);
    cfg.getOption("positionY").set<double>(positionY);
    inventory.saveToConfig(cfg);
}
