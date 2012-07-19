#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Object.hpp"

class ResourceManager;
#include "Tile.hpp"
class Unit;
class Data;

class World;

class LightField;

class Level{
    friend class Unit;
    friend class World;
    private:
        World *world;
        const ResourceManager *resman;
        Tile *data;
        Tile default_tile;
        sf::Vector2u size;
        std::string id;
        int seed;
        bool ready;
        std::map<std::string, sf::Vector2i> landmarks;
    public:
        std::set<Unit*> units;
        std::set<LightField*> lights;
        sf::Color ambient;
    public:
        Level(World *resman, Data data);

        void Create();
        bool IsReady() const;

        const sf::Vector2u& GetSize() const;

        bool InBounds(const sf::Vector2i& pos) const;

        void SetKnown(const sf::Vector2i& pos);
        const TileMemory *GetKnown(const sf::Vector2i& pos) const;
        bool IsKnown(const sf::Vector2i& pos) const;

        const Tile& GetTile(const sf::Vector2i& pos) const;

        //TODO: Add functions for manipulating the map

        bool IsWall(const sf::Vector2i& pos) const;
        bool IsFloor(const sf::Vector2i& pos) const;
        bool BlocksSight(const sf::Vector2i& pos) const;

        void Generate();

        //TODO: GetUnitAt, defriend Unit
        //WIP: Landmarks


        void AddLandmark(const std::string& id, const sf::Vector2i& pos);
        void RemoveLandmark(const std::string& id);
        sf::Vector2i GetLandmark(const std::string& id) const;

        Unit* PlaceUnit(const std::string& unit_type, const sf::Vector2i& pos);
        Unit* PlaceUnit(const std::string& unit_type, const std::string& landmark);

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
        void UpdateLightFields();

        sf::Color GetLightColorAt(const sf::Vector2i& pos) const;
};
#endif
