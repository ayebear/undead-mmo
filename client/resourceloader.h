#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H


class ResourceLoader
{
    public:
        ResourceLoader();
        void LoadFiles();
    private:
        void LoadFonts();
        void LoadImages();
        void LoadSound();
        void LoadTiles();
        sf::Font defaultFontMono;
        sf::Font defaultFontBold;
        static vector<sf::Texture> textures;

};

#endif // RESOURCELOADER_H
