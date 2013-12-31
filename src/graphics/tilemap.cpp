// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "tilemap.h"
#include <iostream>
#include <fstream>
#include "packet.h"

using namespace std;

TileMap::TileMap()
{
    ready = false;
    mapWidth = 0;
    mapHeight = 0;
    mapWidthPx = 0;
    mapHeightPx = 0;
}

TileMap::TileMap(const TileIDVector2D& mapData)
{
    loadFromMemory(mapData);
}

TileMap::TileMap(const string& filename)
{
    loadFromFile(filename);
}

sf::Uint32 TileMap::getWidth() const
{
    return mapWidth;
}

sf::Uint32 TileMap::getHeight() const
{
    return mapHeight;
}

sf::Uint32 TileMap::getWidthPx() const
{
    return mapWidthPx;
}

sf::Uint32 TileMap::getHeightPx() const
{
    return mapHeightPx;
}

bool TileMap::isReady() const
{
    return ready;
}

void TileMap::loadFromMemory(const TileIDVector2D& mapData)
{
    tiles.resize(mapData.size());
    for (unsigned int y = 0; y < mapData.size(); y++)
    {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
            tiles[y].emplace_back(mapData[y][x], x * Tile::tileWidth, y * Tile::tileHeight);
    }

    updateMapSize();

    ready = true;
}

bool TileMap::loadFromFile(const string& filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cerr << "Error loading map file: \"" << filename << "\"\n";
        return false;
    }

    TileID tmpID = 0;
    int width, height;
    inFile >> width >> height;

    tiles.resize(height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            inFile >> tmpID;
            tiles[y].emplace_back(tmpID, x * Tile::tileWidth, y * Tile::tileHeight);
        }
    }
    inFile.close();

    updateMapSize();

    cout << "Loaded map \"" << filename << "\". Size: " << mapWidth << " by " << mapHeight << ".\n";

    ready = true;

    return true;
}

void TileMap::loadFromPacket(sf::Packet& packet)
{
    // Extract data from packet into a TileIDVector2D
    TileIDVector2D tileIds;
    tileIds.resize(1);
    TileID id;
    sf::Uint32 width, height;
    if (packet >> width >> height)
    {
        while (packet >> id)
        {
            tileIds.back().push_back(id); // Add the tile to the current row
            if (tileIds.back().size() >= width) // If the row is wide enough
                tileIds.resize(tileIds.size() + 1); // Add a new row
        }
    }
    tileIds.resize(height);
    // Load the tiles from the vector in memory
    loadFromMemory(tileIds);
}

void TileMap::saveToPacket(sf::Packet& packet) const
{
    packet << Packet::MapData << mapWidth << mapHeight;
    for (const auto& row: tiles)
    {
        for (const auto& tile: row)
            packet << tile.getID();
    }
}

void TileMap::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    if (!ready)
        return;

    // No need to pass the view in separately, it is already stored inside the window
    sf::View viewWindow = window.getView();

    sf::Vector2f viewSize(viewWindow.getSize());
    sf::Vector2f viewCenter(viewWindow.getCenter());
    sf::FloatRect viewRect(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2, viewSize.x, viewSize.y);

    // Convert coordinates of view to logical tile coordinates
    int startX = viewRect.left / Tile::tileWidth;
    int startY = viewRect.top / Tile::tileHeight;

    int endX = (viewRect.left + viewRect.width) / Tile::tileWidth + 1;
    int endY = (viewRect.top + viewRect.height) / Tile::tileHeight + 1;

    // Check if these values are within bounds of the array
    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX >= (int)tiles.front().size())
        endX = tiles.front().size() - 1;
    if (endY >= (int)tiles.size())
        endY = tiles.size() - 1;

    // Draw all of the tiles within view
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
            window.draw(tiles[y][x]);
    }
}

void TileMap::updateMapSize()
{
    mapWidth = tiles.front().size();
    mapHeight = tiles.size();
    mapWidthPx = mapWidth * Tile::tileWidth - Tile::tileWidth;
    mapHeightPx = mapHeight * Tile::tileHeight - Tile::tileHeight;
}
