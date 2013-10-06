#include "tileset.h"
#include <iostream>

TileSet::TileSet()
{
    tileSize.x = 0;
    tileSize.y = 0;
    tileCount.x = 1;
    tileCount.y = 1;
    useTileCount = true;
}

TileSet::TileSet(const std::string& filename, unsigned int width, unsigned int height, bool smooth)
{
    setTileCount(width, height);
    if (!loadImage(filename, smooth))
        exit(121);
}

TileSet::TileSet(const std::string& filename, const sf::Vector2u& newSize, bool smooth)
{
    setTileCount(newSize);
    if (!loadImage(filename, smooth))
        exit(122);
}

TileSet::~TileSet()
{
    clear();
}

void TileSet::setTileSize(const sf::Vector2u& newSize)
{
    if (validSize(newSize))
    {
        tileSize = newSize;
        useTileCount = false;
    }
}

void TileSet::setTileSize(unsigned int width, unsigned int height)
{
    if (validSize(width, height))
    {
        tileSize.x = width;
        tileSize.y = height;
        useTileCount = false;
    }
}

void TileSet::setTileCount(const sf::Vector2u& newSize)
{
    if (validSize(newSize))
    {
        tileCount = newSize;
        useTileCount = true;
    }
}

void TileSet::setTileCount(unsigned int width, unsigned int height)
{
    if (validSize(width, height))
    {
        tileCount.x = width;
        tileCount.y = height;
        useTileCount = true;
    }
}

bool TileSet::loadImage(const std::string& filename, bool smooth)
{
    // Load the entire image into memory
    std::cout << "TileSet loading \"" << filename << "\"... ";
    sf::Image tilesImage;
    bool status = tilesImage.loadFromFile(filename);
    if (status)
    {
        calculateDimensions(tilesImage.getSize()); // Calculates tileSize and tileCount based on the image size
        textures.resize(tileCount.x * tileCount.y); // Resize the textures vector to fit all of the textures
        for (unsigned int y = 0, i = 0; status && y < tileCount.y; y++)
        {
            for (unsigned int x = 0; status && x < tileCount.x; x++, i++)
            {
                sf::IntRect areaToLoad = sf::IntRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y);
                if (textures[i].loadFromImage(tilesImage, areaToLoad))
                    textures[i].setSmooth(smooth);
                else
                    status = false;
            }
        }
    }
    std::cout << (status ? "Done.\n" : "Error!\n");
    return status;
}

void TileSet::setSmooth(bool smooth)
{
    for (auto& tex: textures)
        tex.setSmooth(smooth);
}

void TileSet::clear()
{
    textures.clear();
}

sf::Texture& TileSet::operator[](unsigned int i)
{
    if (textures.empty())
        exit(120); // std::optional could fix this problem in C++14
    if (i >= textures.size())
        i = textures.size() - 1;
    return textures[i];
}

unsigned int TileSet::size() const
{
    return textures.size();
}

bool TileSet::empty() const
{
    return textures.empty();
}

void TileSet::calculateDimensions(const sf::Vector2u& imageSize)
{
    if (useTileCount)
    {
        tileSize.x = imageSize.x / tileCount.x;
        tileSize.y = imageSize.y / tileCount.y;
    }
    else
    {
        tileCount.x = imageSize.x / tileSize.x;
        tileCount.y = imageSize.y / tileSize.y;
    }
}

bool TileSet::validSize(const sf::Vector2u& newSize)
{
    return (newSize.x > 0 && newSize.y > 0);
}

bool TileSet::validSize(unsigned int width, unsigned int height)
{
    return (width > 0 && height > 0);
}
