// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "tile.h"

class Map: public sf::Drawable
{
    public:
        Map();
        Map(std::vector< std::vector<TileID> > & mapData);
        Map(const std::string&);

        int getMapWidth();
        int getMapHeight();
        bool loadMapFromFile(const std::string&);
        void loadMapFromMemory(std::vector< std::vector<TileID> > & mapData);
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        bool ready;
        int mapHeight;
        int mapWidth;
        std::vector<std::vector<Tile>> tiles;
};

#endif
