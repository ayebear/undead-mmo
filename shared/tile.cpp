// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
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
    if (!textures.empty())
    {
        std::cout << "Tile textures already loaded.\n";
        return;
    }

    std::cout << "Loading tile textures...\n";
    // Load the entire image into memory
    sf::Image* tilesImage = new sf::Image;
    tilesImage->loadFromFile("data/images/tiles/tiles.png");
    // Split up the image into separate textures on your GPU
    int tileCount = 1024 / tileWidth; // 16 = 1024 / 128
    textures.resize(tileCount * tileCount);
    for (int y = 0; y < tileCount; y++)
    {
        for (int x = 0; x < tileCount; x++)
        {
            if (!textures[y * tileCount + x].loadFromImage(*tilesImage,
                sf::IntRect(x * tileWidth, y * tileWidth,
                (x + 1) * tileWidth, (y + 1) * tileWidth)))
                    exit(Errors::Graphics);
            textures[y * tileCount + x].setSmooth(false);
        }
    }
    // Delete the image from memory
    delete tilesImage;
    std::cout << "Done loading tile textures!\n";
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
    //else
        //std::cout << "Fatal Error: Failed to set texture. textures.size() = " << textures.size() << "\n";
}

const TileID Tile::GetID() const
{
    return ID;
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
