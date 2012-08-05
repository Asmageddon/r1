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

struct Stat {
    float value;
    float variation;
    float potential;

    Stat() { /* ... */};
    Stat(Data data, const std::string& stat_name);
};

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

        AString ai;
        AString ai_swap_policy;

        int movement_speed;
        int perception_min;
        int perception_max;

        //Placeholder until I decide to do groups/civilizations
        bool enemy;

        Stat strength;
        Stat agility;
        Stat dexterity;
        Stat endurance;

        float size; //Body size
    public:
        UnitType() { /* WARNING: This does not initialize anything */ };
        UnitType(ResourceManager *resman, Data data);
};

#endif
