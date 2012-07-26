#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <set>
#include "AString.hpp"

#include <SFML/Graphics.hpp>

class UnitType;
class Level;
class SightField;
class LightField;
class Material;

class World;

class AI;
class Action;

class Unit {
    private:
        std::set<LightField*> lights;
        SightField *fov;
        sf::Vector2i pos;
        World *world;
        Level *location;
        AI *ai;
        Action *next_action;
    public:
        const UnitType *type;
        const Material *material;

        //TODO: hp, integrity, temperature, etc.

    public:
        Unit(World *world, const std::string& type_id);
        ~Unit();

        //TODO: Setting and getting material and type
        //TODO: Setting and getting rest of stuff

        //These are actions, they should return success or failure
        bool Swap(Unit *other_unit);

        bool Move(const sf::Vector2i& vec);

        void SetPosition(const sf::Vector2i& pos, bool ignore_terrain = false);
        void SetPosition(const std::string& landmark, bool ignore_terrain = false);
        const sf::Vector2i& GetPosition() const;

        void SetLocation(const std::string& loc_id, const sf::Vector2i pos = sf::Vector2i(0,0), bool ignore_terrain = false);
        void SetLocation(const std::string& loc_id, const std::string& landmark, bool ignore_terrain = false);
        AString GetLocation() const;

        Level *GetCurrentLevel();

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);

        bool CanSee(const sf::Vector2i& pos);
        bool CanSee(const Unit* const unit);
        float GetLightThreshold() const;

        void RecalculateFOV();

        void SetNextAction(Action *action, bool interrupt=true);
        const Action* GetNextAction() const;

        void AttachAI(AI *ai);
        void DetachAI();
        AI *GetAI();

        void Simulate(); //Don't call this directly
};
#endif
