// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "tile.h"

typedef std::vector< std::vector<TileID> > TileIDVector2D;
typedef std::vector< std::vector<Tile> > TileVector2D;

// This class is used for drawing a graphical tile map
// TODO: Separate the graphics code so that this can be used on the server without graphics
// TODO: Make this more generic and flexible so you can use custom tile types and stuff
class TileMap: public sf::Drawable
{
    public:
        TileMap();
        TileMap(const TileIDVector2D&);
        TileMap(const std::string&);

        sf::Uint32 getWidth() const;
        sf::Uint32 getHeight() const;
        sf::Uint32 getWidthPx() const;
        sf::Uint32 getHeightPx() const;
        bool isReady() const;
        void loadFromMemory(const TileIDVector2D& mapData);
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
