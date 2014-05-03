#include "components.h"
#include <unordered_map>
#include <SFML/Graphics.hpp>

std::unordered_map<std::string, sf::Texture> Renderable::textures;

void Renderable::setTexture(const std::string& filename)
{
    if (!filename.empty())
    {
        if (textures.find(filename) == textures.end())
        {
            if (textures[filename].loadFromFile(filename))
                sprite.setTexture(textures[filename]);
            else
                std::cerr << "Could not load texture from file " << filename << std::endl;
        }
        else
            sprite.setTexture(textures[filename]);
    }

    textureFile = filename;
}
