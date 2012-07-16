#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <set>

#include <SFML/Graphics.hpp>

class UnitType;
class Level;
class LightField;
class ResourceManager;
class Material;

class Unit {
    public:
        const UnitType *type;
        const Material *material;

        //TODO: hp, integrity, temperature, etc.

        Level *location;
        std::set<LightField*> lights;
        ResourceManager *resman;

        sf::Vector2i pos;

    public:
        Unit(ResourceManager* resman, const std::string& type_id);

        void Move(const sf::Vector2i& vec);

        void SetPosition(const sf::Vector2i& pos);
        const sf::Vector2i& GetPosition() const;

        void SetLocation(Level *location);
        //void SetLocation(const std::string& loc);
        //const std::string& GetLocation() const;

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
};
#endif