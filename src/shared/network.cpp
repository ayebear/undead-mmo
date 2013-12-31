// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "network.h"
#include <SFML/Network.hpp>
#include <iostream>

bool Network::downloadFile(const std::string& url, std::string& out)
{
    bool status = false;

    std::cout << "Downloading " << url << "... ";

    auto slash = url.find('/');
    if (slash != std::string::npos)
    {
        std::string host = url.substr(0, slash);
        std::string path = url.substr(slash);

        sf::Http http(host);
        sf::Http::Request request(path);
        sf::Http::Response response = http.sendRequest(request);

        if (response.getStatus() == sf::Http::Response::Ok)
        {
            out = response.getBody();
            status = true;
        }
    }

    if (status)
        std::cout << "Successful!\n";
    else
        std::cout << "Error.\n";

    return status;
}
