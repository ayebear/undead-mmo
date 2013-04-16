// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MAP_H
#define MAP_H

#include <deque>
#include "tile.h"

class Map
{
    public:
        Map();
    private:
        std::deque<Tile> tiles;
};

#endif // MAP_H
