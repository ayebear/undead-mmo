#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

// This class allows you to load a tile set into memory from a single image file
class TileSet
{
    public:
        static const bool defaultSmooth = false;

        TileSet();
        TileSet(const std::string&, unsigned int, unsigned int, bool smooth = defaultSmooth); // Sets the tile count
        TileSet(const std::string&, const sf::Vector2u&, bool smooth = defaultSmooth); // Sets the tile count
        ~TileSet();

        void setTileSize(const sf::Vector2u&); // Set the absolute tile size in pixels
        void setTileSize(unsigned int, unsigned int);
        void setTileCount(const sf::Vector2u&); // Set the number of tiles to be loaded from the image
        void setTileCount(unsigned int, unsigned int);
        bool loadImage(const std::string&, bool smooth = defaultSmooth); // Load an image
        void setSmooth(bool smooth = defaultSmooth); // Set anti-aliasing of all loaded textures
        void clear(); // Clears all of the textures

        sf::Texture& operator[](unsigned int); // Returns a reference to a texture in the vector
        unsigned int size() const; // Returns number of textures
        bool empty() const; // Returns true if there are no textures

    private:
        void calculateDimensions(const sf::Vector2u&);
        bool validSize(const sf::Vector2u&);
        bool validSize(unsigned int, unsigned int);

        sf::Vector2u tileSize; // Tile size in pixels
        sf::Vector2u tileCount; // Number of tiles to load from the image
        bool useTileCount; // To determine whether to use the tile count or tile size
        std::vector<sf::Texture> textures; // The loaded image is split into separate textures stored in here
};

#endif
