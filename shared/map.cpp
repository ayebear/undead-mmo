// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include <fstream>
#include "map.h"

using namespace std;

Map::Map()
{
    ready = false;
    mapHeight = 0;
    mapWidth = 0;
    Tile::loadTileTextures();
}

Map::Map(vector<vector<TileID> > & mapData)
{
    Tile::loadTileTextures();
    loadMapFromMemory(mapData);
}

Map::Map(const string& filename)
{
    Tile::loadTileTextures();
    loadMapFromFile(filename);
}

int Map::getMapHeight()
{
    return mapHeight;
}

int Map::getMapWidth()
{
    return mapWidth;
}

void Map::loadMapFromMemory(vector<vector<TileID> > & mapData)
{
    tiles.resize(mapData.size());
    for (unsigned int y = 0; y < mapData.size(); y++)
    {
        for (unsigned int x = 0; x < mapData[y].size(); x++)
        {
            tiles[y].emplace_back(mapData[y][x], x * Tile::tileWidth, y * Tile::tileHeight);
        }
    }
    mapHeight = tiles.size() * Tile::tileHeight - Tile::tileHeight;
    mapWidth = tiles.front().size() * Tile::tileWidth - Tile::tileWidth;

    ready = true;
}

bool Map::loadMapFromFile(const string& filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return false;

    string temp;
    int tmpID = 0;
    int width, height;
    in >> width >> height;

    tiles.resize(height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            in >> tmpID;
            tiles[y].emplace_back(tmpID, x * Tile::tileWidth, y * Tile::tileHeight);
        }
    }
    in.close();

    mapHeight = tiles.size() * Tile::tileHeight - Tile::tileHeight;
    mapWidth = tiles.front().size() * Tile::tileWidth - Tile::tileWidth;

    ready = true;

    return true;
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
