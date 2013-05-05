// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "tile.h"

std::vector<sf::Texture>* Tile::textures = nullptr;

Tile::Tile()
{
    SetID(0);
    SetPos(0, 0);
}

Tile::Tile(TileID tileID, int x, int y)
{
    SetID(tileID);
    SetPos(x, y);
}

void Tile::SetTexturesPtr(std::vector<sf::Texture>* texturesPtr)
{
    textures = texturesPtr;
}

void Tile::SetID(TileID tileID)
{
    ID = tileID;

    if (ID < 128)
        walkable = true;
    else
        walkable = false;

    if (textures == nullptr)
        exit(555);

    sprite.setTexture((*textures)[ID]);
}

void Tile::SetPos(int x, int y)
{
    sprite.setPosition(x, y);
}

bool Tile::IsWalkable()
{
    return walkable;
}

void Tile::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}
