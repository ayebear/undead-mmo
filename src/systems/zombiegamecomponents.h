#ifndef ZOMBIEGAMECOMPONENTS_H
#define ZOMBIEGAMECOMPONENTS_H


#include <unordered_map>

#include <OCS/Component.hpp>
#include <SFML/Graphics/Rect.hpp>

struct Position : public ocs::Component<Position>
{
    Position(float _x = 0.0f, float _y = 0.0f) : 
        x(_x), 
        y(_y) 
        {}

    std::string serialize() { serializer.serialize("% %", x, y); }
    
    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, x, y); }
    
    float x, y;
};

struct Velocity : public ocs::Component<Velocity>
{
    Velocity(float dx = 0.0f, float dy = 0.0f) : dx(dx), dy(dy) {}

    std::string serialize() { serializer.serialize("% %", dx, dy); }
    
    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, dx, dy); }

    float dx, dy;
};


////////////////////////////////////////////////////Stat

enum ValueType
{
    MIN = 0,
    CURRENT,
    MAX,
    TOTAL
};

struct StatModInfo
{
    StatModInfo()

    std::string statName;
    short valueType; //Min, Current, or Max
    int value; // Amount to modify
};

struct StatModifier : public ocs::Component<StatModifier>
{
    StatModifier() {}

    std::vector<StatModInfo> modifiedStats;
};



struct Stat
{
    Stat(int min, int current, int max) :
        data[ValueType::MIN](min),
        data[ValueType::CURRENT](current),
        data[ValueType::MAX](max)
    {}

    int data[ValueType::TOTAL];
};

struct StatMap : public ocs::Component<StatMap>
{
    std::unordered_map<std::string, Stat> stats;
};

//////////////////////////////////////////////End Stat

struct Collidable : public ocs::Component<Collidable>
{

};

struct RigidBody : public ocs::Component<RigidBody> {}

struct Renderable : public ocs::Component<Renderable>
{
    static std::unordered_map<std::string, sf::Texture> textures;

    Renderable(const std::string& txtreFile = "")
    {
        if (txtreFile != "")
        {
            if(textures.find(txtreFile) == textures.end())
            {
                if(textures[txtreFile].loadFromFile(txtreFile))
                    sprite.setTexture(textures[txtreFile]);
                else
                    std::cerr << "Could not load texture from file " << txtreFile << std::endl;
            }
            else
                sprite.setTexture(textures[txtreFile]);
        }

        textureFile = txtreFile;
    }

    std::string serialize() { serializer.serialize("%", textureFile); }

    void deSerialize(const std::string& str)
    {
        std::string txtreFile;
        serializer.deSerialize("%s", str, txtreFile);

        if (txtreFile != "")
        {
            if (textures.find(txtreFile) == textures.end())
            {
                if (textures[txtreFile].loadFromFile(txtreFile))
                    sprite.setTexture(textures[txtreFile]);
                else
                    std::cerr << "Could not load texture from file " << txtreFile << std::endl;
            }
            else
                sprite.setTexture(textures[txtreFile]);
        }
    }

    sf::Sprite sprite;
    std::string textureFile;
};

#endif
