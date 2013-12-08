// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "tile.h"

typedef std::vector< std::vector<TileID> > TileIDVector2D;
typedef std::vector< std::vector<Tile> > TileVector2D;

class Map: public sf::Drawable
{
    public:
        Map();
        Map(TileIDVector2D& mapData);
        Map(const std::string&);

        sf::Uint32 getWidth() const;
        sf::Uint32 getHeight() const;
        sf::Uint32 getWidthPx() const;
        sf::Uint32 getHeightPx() const;
        bool isReady() const;
        void loadFromMemory(TileIDVector2D& mapData);
        bool loadFromFile(const std::string&);
        void loadFromPacket(sf::Packet&);
        void saveToPacket(sf::Packet&) const;
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void updateMapSize();

        bool ready;
        sf::Uint32 mapWidth, mapHeight;
        sf::Uint32 mapWidthPx, mapHeightPx;
        TileVector2D tiles;
};

#endif
