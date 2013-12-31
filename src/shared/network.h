// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef NETWORK_H
#define NETWORK_H

#include <string>

// TODO: Figure out a better solution for UDP ports...
const unsigned short serverPort = 1337;
const unsigned short clientPort = 1338;

namespace Network
{
    bool downloadFile(const std::string&, std::string&);
}

#endif
