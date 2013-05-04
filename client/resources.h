// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>
#include <SFML/Graphics.hpp>

class Resources
{
    public:
        Resources();
        static void LoadFiles();
        static sf::Font font;
        static sf::Font fontBold;
        static sf::Font fontMono, fontMonoBold;
        static std::vector<sf::Texture> textures;
        static const int tilesImageSize;
        // Temporary character textures and stuff (untill we setup the proper sprite sheets)
        static sf::Texture playerTex, zombieTex, menuBgTex;

    private:
        static void LoadFonts();
        static void LoadImages();
        static void LoadTiles();
        static void LoadSounds();
};

#endif
