// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include "tileset.h"

typedef sf::Uint16 TileID;

class Tile: public sf::Drawable
{
    public:
        Tile();
        Tile(TileID, int, int);
        void setID(TileID);
        const TileID getID() const;
        void setPos(int, int);
        bool isWalkable() const;
        void draw(sf::RenderTarget&, sf::RenderStates) const;

        static const unsigned int tileWidth = 128;
        static const unsigned int tileHeight = 128;
        static void loadTextures();

    private:
        static TileSet textures;

        TileID ID;
        bool walkable;
        sf::Sprite sprite;
};

#endif
