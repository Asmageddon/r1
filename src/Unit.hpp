#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <set>

#include <SFML/Graphics.hpp>

class UnitType;
class Level;
class SightField;
class LightField;
class Material;

class World;

class Unit {
    private:
        std::set<LightField*> lights;
        SightField *fov;
        sf::Vector2i pos;
        World *world;
        Level *location;
    public:
        const UnitType *type;
        const Material *material;

        //TODO: hp, integrity, temperature, etc.

    public:
        Unit(World *world, const std::string& type_id);
        ~Unit();

        //TODO: Setting and getting material and type
        //TODO: Setting and getting rest of stuff

        void Swap(Unit *other_unit);

        void Move(const sf::Vector2i& vec);

        void SetPosition(const sf::Vector2i& pos, bool ignore_terrain = false);
        void SetPosition(const std::string& landmark, bool ignore_terrain = false);
        const sf::Vector2i& GetPosition() const;

        void SetLocation(const std::string& loc_id, const sf::Vector2i pos = sf::Vector2i(0,0), bool ignore_terrain = false);
        void SetLocation(const std::string& loc_id, const std::string landmark, bool ignore_terrain = false);
        std::string GetLocation() const;

        Level *GetCurrentLevel();

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);

        bool CanSee(const sf::Vector2i& pos);
        bool CanSee(const Unit* const unit);
        float GetLightThreshold() const;

        void RecalculateFOV();
};
#endif
