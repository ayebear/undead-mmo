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
        void SetID(TileID);
        void SetPos(int, int);
        bool IsWalkable();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

        static void SetTexturesPtr(std::vector<sf::Texture>*);

    private:
        TileID ID;
        bool walkable;
        sf::Sprite sprite;

        static std::vector<sf::Texture>* textures;
};

#endif
