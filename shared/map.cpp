// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "map.h"
#include <iostream>
#include <fstream>
#include "packet.h"

using namespace std;

Map::Map()
{
    ready = false;
    mapWidth = 0;
    mapHeight = 0;
    mapWidthPx = 0;
    mapHeightPx = 0;
}

Map::Map(TileIDVector2D& mapData, bool loadTextures)
{
    if (loadTextures)
        Tile::loadTileTextures();
    loadFromMemory(mapData);
}

Map::Map(const string& filename, bool loadTextures)
{
    if (loadTextures)
        Tile::loadTileTextures();
    loadFromFile(filename);
}

sf::Uint32 Map::getWidth()
{
    return mapWidth;
}

sf::Uint32 Map::getHeight()
{
    return mapHeight;
}

sf::Uint32 Map::getWidthPx()
{
    return mapWidthPx;
}

sf::Uint32 Map::getHeightPx()
{
    return mapHeightPx;
}

bool Map::isReady()
{
    return ready;
}

void Map::loadFromMemory(TileIDVector2D& mapData)
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

bool Map::loadFromFile(const string& filename)
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

void Map::loadFromPacket(sf::Packet& packet)
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

void Map::saveToPacket(sf::Packet& packet)
{
    packet << Packet::MapData << mapWidth << mapHeight;
    for (auto& row: tiles)
    {
        for (auto& tile: row)
            packet << tile.getID();
    }
}

void Map::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    if (!ready)
        return;

    // No need to pass the view in separately, it is already stored inside the window
    sf::View viewWindow = window.getView();
    // TODO: Actually calculate the rectangle from the sf::View of the window
    //sf::FloatRect viewRect(0, 0, 800, 600);

    sf::Vector2f viewSize(viewWindow.getSize());
    sf::Vector2f viewCenter(viewWindow.getCenter());
    sf::FloatRect viewRect(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2, viewSize.x, viewSize.y);

    // Convert coordinates of view to logical tile coordinates
    int startX = viewRect.left / Tile::tileWidth;
    int startY = viewRect.top / Tile::tileHeight;
    //int startX =  (viewRect.top - ((int)viewRect.top % (int)tileHeight)) / tileHeight;//viewRect.left / tileWidth;
    //int startY =  (viewRect.left - ((int)viewRect.left % (int)tileWidth)) / tileWidth;

    int endX = (viewRect.left + viewRect.width) / Tile::tileWidth + 1;
    int endY = (viewRect.top + viewRect.height) / Tile::tileHeight + 1;
    //int endX =  (viewRect.top + viewRect.height + (tileHeight - (int)(viewRect.top + viewRect.height) % tileHeight)) / tileHeight;// (viewRect.left + viewRect.width) / tileWidth + 1;
    //int endY =  (viewRect.left + viewRect.width + (tileWidth - (int)(viewRect.left + viewRect.width) % tileWidth)) / tileWidth;//(viewRect.top + viewRect.height) / tileHeight + 1;

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
        {
            window.draw(tiles[y][x]);
        }
    }
}

void Map::updateMapSize()
{
    mapWidth = tiles.front().size();
    mapHeight = tiles.size();
    mapWidthPx = mapWidth * Tile::tileWidth - Tile::tileWidth;
    mapHeightPx = mapHeight * Tile::tileHeight - Tile::tileHeight;
}
