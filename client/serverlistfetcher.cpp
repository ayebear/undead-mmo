#include "serverlistfetcher.h"
#include "stringutils.h"
#include "network.h"
#include <iostream>

ServerListFetcher::ServerListFetcher()
{
}

ServerListFetcher::ServerListFetcher(const std::string& filename)
{
    loadMasterServers(filename);
}

bool ServerListFetcher::loadMasterServers(const std::string& filename)
{
    return StringUtils::readLinesFromFile(filename, masterServers, false);
}

bool ServerListFetcher::refresh()
{
    // Loop through all of the master servers until one works
    serverList.clear();
    std::string serverListContents;
    for (const std::string& mServer: masterServers)
    {
        if (Network::downloadFile(mServer, serverListContents))
        {
            std::cout << "1\n";
            serverList.loadFromString(serverListContents);
            std::cout << "2\n";
            return true;
        }
    }
    return false;
}

CsvFile& ServerListFetcher::getServerList()
{
    return serverList;
}
