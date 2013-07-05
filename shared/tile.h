// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TILE_H
#define TILE_H

#include <vector>
#include <SFML/Graphics.hpp>

typedef sf::Uint16 TileID;

class Tile: public sf::Drawable
{
    public:
        Tile();
        Tile(TileID, int, int);
        void setID(TileID);
        const TileID getID() const;
        void setPos(int, int);
        bool isWalkable();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

        static const int tileWidth = 128;
        static const int tileHeight = 128;
        static void loadTileTextures();

    private:
        TileID ID;
        bool walkable;
        sf::Sprite sprite;
        static std::vector<sf::Texture> textures;
};

#endif
