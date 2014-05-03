#ifndef ZOMBIEGAMECOMPONENTS_H
#define ZOMBIEGAMECOMPONENTS_H


#include <unordered_map>

#include <OCS/Components.hpp>
#include <SFML/Graphics.hpp>

struct Position : public ocs::Component<Position>
{
    Position(float _x = 0.0f, float _y = 0.0f) :
        x(_x),
        y(_y)
        {}

    std::string serialize() { return serializer.serialize("% %", x, y); }

    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, x, y); }

    float x, y;
};

struct Velocity : public ocs::Component<Velocity>
{
    Velocity(float dx = 0.0f, float dy = 0.0f) : dx(dx), dy(dy) {}

    std::string serialize() { return serializer.serialize("% %", dx, dy); }

    void deSerialize(const std::string& str) { serializer.deSerialize("% %", str, dx, dy); }

    float dx, dy;
};


////////////////////////////////////////////////////Stat

struct StatModInfo
{
    StatModInfo();

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
    enum ValueType
    {
        MIN = 0,
        CURRENT,
        MAX,
        TOTAL
    };

    Stat(int min, int current, int max) :
        data{min, current, max}
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

struct Renderable : public ocs::Component<Renderable>
{
    static std::unordered_map<std::string, sf::Texture> textures;

    Renderable(const std::string& filename = "")
    {
        setTexture(filename);
    }

    std::string serialize() { return serializer.serialize("%", textureFile); }

    void deSerialize(const std::string& str)
    {
        std::string filename;
        serializer.deSerialize("%s", str, filename);
        setTexture(filename);
    }

    sf::Sprite sprite;
    std::string textureFile;

    private:

        void setTexture(const std::string& filename);
};

#endif
