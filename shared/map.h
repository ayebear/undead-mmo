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
