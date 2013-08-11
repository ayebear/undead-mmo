// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "ipport.h"

bool IpPort::operator<(const IpPort& addr) const
{
    if (ip == addr.ip)
        return (port < addr.port && port < 1338 && 1338 < addr.port); // TODO: Remove this check and have 2 IpPorts for each client
    else
        return (ip < addr.ip);
}

bool IpPort::operator==(const IpPort& addr) const
{
    return (ip == addr.ip && port == addr.port);
}
