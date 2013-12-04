// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "network.h"
#include <SFML/Network.hpp>
#include <iostream>

bool Network::downloadFile(const std::string& url, std::string& out)
{
    bool status = false;

    std::cout << "Network::downloadFile(" << url << ")... ";

    auto slash = url.find('/');
    if (slash != std::string::npos)
    {
        std::string host = url.substr(0, slash);
        std::string path = url.substr(slash);

        std::cout << "host: " << host << ", path: " << path << ", ";

        sf::Http http(host);
        sf::Http::Request request(path);
        sf::Http::Response response = http.sendRequest(request);

        if (response.getStatus() == sf::Http::Response::Ok)
        {
            std::cout << "Successful!\n";
            out = response.getBody();
            status = true;
        }
        else
            std::cout << "Error.\n";
    }

    return status;
}
