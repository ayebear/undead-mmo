#ifndef SERVERLISTFETCHER_H
#define SERVERLISTFETCHER_H

#include <string>
#include <vector>
//#include "csvfile.h"
#include "configfile.h"

class ServerListFetcher
{
    public:
        ServerListFetcher();
        ServerListFetcher(const std::string&);
        bool loadMasterServers(const std::string&);
        bool refresh();
        cfg::File& getServerList();

    private:
        std::vector<std::string> masterServers;
        cfg::File serverList;    // Updated for configFile
};

#endif
