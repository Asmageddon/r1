#ifndef UNITTYPE_HPP_
#define UNITTYPE_HPP_

#include "../AString.hpp"
#include <vector>

#include <SFML/Graphics.hpp>

#include "Resource.hpp"
#include "Displayable.hpp"
#include "Glowable.hpp"

class ResourceManager;
class Data;

class UnitType : public Resource, public Displayable, public Glowable {
    private:
        ResourceManager *resman;
    public:
        AString type;
        std::vector<AString> categories;
        AString material;

        int sight_radius;
        float sight_threshold;
        sf::Color vision_tint;

        bool travel_walls;
        bool travel_floor;
        unsigned int travel_conditions;

        //WIP:
        AString ai;
        AString ai_swap_policy;

    public:
        UnitType() { /* WARNING: This does not initialize anything */ };
        UnitType(ResourceManager *resman, Data data);
};

#endif
