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

        bool ready;
        static const int tileWidth = 128;
        static const int tileHeight = 128;

        //Used to hold the tileIDs from the map. 1 0 3 0 2 1 1 2 3 etc.
        //std::vector<std::vector<int>> mapData;
        // Isn't this the same as the ID variable in the tile struct?

        std::vector<std::vector<Tile>> tiles;

        //Used to hold the separate textures for each tile. textures[0] will be first tile and so on.
        std::vector<sf::Texture> textures;

        //Used hold tiles.png. Load the textures vector from this variable
        //sf::Texture tileImages;
        // Uses less memory to split it up directly from the file
};

#endif
