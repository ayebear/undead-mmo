// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "tile.h"
#include "other.h"

std::vector<sf::Texture> Tile::textures;

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

void Tile::LoadTileTextures()
{
    // 16 = 1024 / 128
    int tileCount = 1024 / tileWidth;
    textures.resize(tileCount * tileCount);
    for (int y = 0; y < tileCount; y++)
    {
        for (int x = 0; x < tileCount; x++)
        {
            if (!textures[y * tileCount + x].loadFromFile("data/images/tiles/tiles.png",
                sf::IntRect(x * tileWidth, y * tileWidth,
                (x + 1) * tileWidth, (y + 1) * tileWidth)))
                    exit(Errors::Graphics);
            textures[y * tileCount + x].setSmooth(false);
        }
    }
}

void Tile::SetID(TileID tileID)
{
    ID = tileID;

    if (ID < 128)
        walkable = true;
    else
        walkable = false;

    if (ID < textures.size())
        sprite.setTexture(textures[ID]);
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
