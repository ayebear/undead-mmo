// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "playerdata.h"

PlayerData::PlayerData()
    : salt(0),
    banned(false),
    health(0),
    level(0)
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
}

void PlayerData::saveToConfig(ConfigFile& cfg) const
{
    cfg.getOption("username").setString(username);
    cfg.getOption("password").setString(passwordHash);
    cfg.getOption("salt").setInt(salt);
    cfg.getOption("health").setInt(health);
    cfg.getOption("level").setInt(level);
    cfg.getOption("banned").setBool(banned);
}
