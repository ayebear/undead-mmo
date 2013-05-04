// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "map.h"
#include "../client/resources.h"
#include <fstream>

Tile::Tile(sf::UINT16 tileID, float x, float y)
{
    id = TileID;
    if(tileID < 128)
    {
        walkable = true;
    }
    else
    {
        walkable = false;
    }

    updateImage(tileID);
}

Tile::updateImage(sf::UINT16 tileID)
{
    tileImage.setTexture(Resources::textures[tileID]);
    tileImage.setPosition(x, y);
}


Map::drawMap(sf::RenderWindow& window, sf::Rect viewWindow)
{

    //Convert coordinates for view.
    int startX = viewWindow.left / tileWidth;
    int startY = viewWindow.top / tileHeight;

    int endX = (viewWindow.left + viewWindow.width) / tileWidth + 1;
    int endY = (viewWindow.top + viewWindow.height) / tileHeight + 1;

    if(endX > tiles[startX].size() && endY >)
        for(int y = startY; x < endY; x++)
        {
            for(int x = startX; X < endX; y++)
            {
                window.draw(tiles[y][x]);
            }
        }
}

void Map::loadMapFromFile(char*)
{
    istream in;
    string temp;
    int tmpID = 0;
    int width;
    int hieght;
    in >> width >> height;
    std::vector<std::vector<short>> ids;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            in >> tmpID;
            ids.push_back(tmpID);
            tiles.push_back(Tile(ids[i][j], i * tileWidth, j * tileHeight));
        }
    }

}
void Map::loadMapFromMemory(std::vector<std::vector<sf::UINT16 id>>& mapData)
{
    for(int i = 0; i < mapData.size(); i++)
    {
        for(int j = 0; j < mapData[i].size(); j++)
        {
            tiles.push_back(Tile(mapData[i][j], i * tileWidth, j * tileHeight));
        }
    }
}

Map::Map(char* mapFile)
{
    istream in;
    string temp;
    int tmpID = 0;
    int width;
    int hieght;
    in >> width >> height;
    std::vector<std::vector<short>> ids;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            in >> tmpID;
            ids.push_back(tmpID);
            tiles.push_back(Tile(ids[i][j], i * tileWidth, j * tileHeight));
        }
    }

}
Map::Map(std::vector<std::vector<sf::UINT16 id>>& mapData)
{
    for(int i = 0; i < mapData.size(); i++)
    {
        for(int j = 0; j < mapData[i].size(); j++)
        {
            tiles.push_back(Tile(mapData[i][j], i * tileWidth, j * tileHeight));
        }
    }

}
