// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MAP_H
#define MAP_H

#include <vector>
#include "SFML/Graphics.hpp"

struct Tile: public sf::Drawable{

    sf::UINT16 id;
    bool walkable;
    void updateImage();

    Tile(sf::Uint16);

    sf::Sprite tileImage;

};

class Map
{
    public:

        static const tileWidth = 128;
        static const tileHeight = 128;
        Map(std::vector<std::vector<sf::UINT16 id>>& mapData);
        Map(char*);

        void loadMapFromFile(char*);
        void loadMapFromMemory(std::vector<std::vector<sf::UINT16 id>>& mapData);

        //uint16 is a tile ID
        void drawMap(sf::RenderWindow& window, sf::Rect viewWindow);

    private:
        std::vector<std::vector<Tile*>> tiles;
};

#endif // MAP_H
