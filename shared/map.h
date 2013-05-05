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

        bool LoadMapFromFile(const std::string&);
        void LoadMapFromMemory(std::vector< std::vector<TileID> > & mapData);

        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void LoadTileTextures();

        std::vector<std::vector<Tile>> tiles;
        bool ready;
        static const int tileWidth = 128;
        static const int tileHeight = 128;
        std::vector<sf::Texture> textures;
};

#endif
