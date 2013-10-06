// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "tile.h"
#include <iostream>
#include <memory>

//TileSet Tile::textures("data/images/tiles/tiles.png", 8, 8); // Can't do this because we don't want to load them on the server
TileSet Tile::textures;

Tile::Tile()
{
    setID(0);
    setPos(0, 0);
}

Tile::Tile(TileID tileID, int x, int y)
{
    setID(tileID);
    setPos(x, y);
}

void Tile::loadTileTextures()
{
    textures.setTileSize(tileWidth, tileHeight);
    textures.loadImage("data/images/tiles/tiles.png");
}

void Tile::setID(TileID tileID)
{
    ID = tileID;

    walkable = (ID < 128);

    if (ID < textures.size())
        sprite.setTexture(textures[ID]);
    //else
        //std::cout << "Fatal Error: Failed to set texture. textures.size() = " << textures.size() << "\n";
}

const TileID Tile::getID() const
{
    return ID;
}

void Tile::setPos(int x, int y)
{
    sprite.setPosition(x, y);
}

bool Tile::isWalkable() const
{
    return walkable;
}

void Tile::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}
