#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <set>
#include <queue>
#include "AString.hpp"

#include <SFML/Graphics.hpp>

class UnitType;
class Stat;
class Level;
class SightField;
class LightField;
class Material;

class World;

class AI;
class Action;

enum UNIT_STAT {
    //Base stats
    STAT_STRENGTH,
    STAT_AGILITY,
    STAT_DEXTERITY,
    STAT_ENDURANCE,

    //Status
    CURRENT_HP,

    //Computed stats (not settable/trainable)
    STAT_MAX_HP,
    STAT_RESILIENCE,
    STAT_SPEED
};


class Unit {
    private:
        std::set<LightField*> lights;
        SightField *fov;
        sf::Vector2i pos;
        World *world;
        Level *location;
        AI *ai;
        std::queue<Action*> actions;

        float health;

        struct UnitStat {
            float value;
            float potential;
            void operator=(const Stat& type_stat);
        };

        UnitStat strength;
        UnitStat agility;
        UnitStat dexterity;
        UnitStat endurance;
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

        void SetNextAction(Action* action, bool interrupt=true);
        const Action* GetNextAction() const;

        void AttachAI(AI *ai);
        void DetachAI();
        AI *GetAI();

        void Simulate(); //Don't call this directly

        int GetMovementSpeed() const;

        void SetStat(UNIT_STAT stat, float value);
        float GetStat(UNIT_STAT stat) const;

        void SetStatPotential(UNIT_STAT stat, float potential);
        float GetStatPotential(UNIT_STAT stat) const;

        float TrainStat(UNIT_STAT stat, float amount);

        void Die();

        //Returns true if the unit died
        bool Hurt(const std::string& damage_type, float amount);
        void Heal(float amount);
};
#endif
