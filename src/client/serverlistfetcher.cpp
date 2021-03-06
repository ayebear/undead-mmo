#include "serverlistfetcher.h"
#include "strlib.h"
#include "network.h"
#include "paths.h"

ServerListFetcher::ServerListFetcher()
{
    serverList.loadFromFile(Paths::serverListFile);
}

ServerListFetcher::ServerListFetcher(const std::string& filename):
    ServerListFetcher()
{
    loadMasterServers(filename);
}

bool ServerListFetcher::loadMasterServers(const std::string& filename)
{
    return strlib::readLinesFromFile(filename, masterServers, false);
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
            serverList.loadFromString(serverListContents);
            serverList.writeToFile();
            return true;
        }
    }
    return false;
}

cfg::File& ServerListFetcher::getServerList()
{
    return serverList;
}
