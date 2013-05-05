// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "tile.h"

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

void Tile::SetID(TileID tileID)
{
    ID = tileID;

    if (ID < 128)
        walkable = true;
    else
        walkable = false;

    sprite.setTexture(Resources::textures[id]);
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
