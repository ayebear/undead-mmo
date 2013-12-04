#ifndef SERVERLISTFETCHER_H
#define SERVERLISTFETCHER_H

#include <string>
#include <vector>
#include "csvfile.h"

class ServerListFetcher
{
    public:
        ServerListFetcher();
        ServerListFetcher(const std::string&);
        bool loadMasterServers(const std::string&);
        bool refresh();
        CsvFile& getServerList();

    private:
        std::vector<std::string> masterServers;
        CsvFile serverList;
};

#endif
