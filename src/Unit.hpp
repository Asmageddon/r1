#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <set>

#include <SFML/Graphics.hpp>

#include "UnitType.hpp"

class Level;
class LightField;
class ResourceManager;
class Material;

class Unit {
    public:
        const UnitType *type;
        const Material *material;

        u_int16_t integrity;
        u_int16_t temperature;

        u_int32_t hp;

        Level *location;
        std::set<LightField*> lights;
        ResourceManager *resman;

        sf::Vector2i pos;

    public:
        Unit(Level* location, const UnitType *type);

        void Move(const sf::Vector2i& vec);
        //TODO: This v
        //void SetLocation(const std::string& loc);
        //const std::string& GetLocation() const;

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
};
#endif