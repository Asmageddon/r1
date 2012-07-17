#ifndef UNITTYPE_HPP_
#define UNITTYPE_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Object.hpp"
#include "Displayable.hpp"
#include "Glowable.hpp"

class ResourceManager;
class Data;

//TODO: Write getters for all these properties. I don't want unit types mutable unless under special circumstances

class UnitType : public Object, public Displayable, public Glowable {
    public:
        std::string type;
        std::vector<std::string> categories;
        std::string material;

        int sight_radius;
        float sight_threshold;
        sf::Color vision_tint;

        ResourceManager *resman;
    public:
        UnitType() { /* WARNING: This does not initialize anything */ };
        UnitType(ResourceManager *resman, Data data);
};

#endif