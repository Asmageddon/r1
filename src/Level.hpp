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
        std::map<std::string, Tile> tiles;
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
        //TODO: Add drawing functions
        void SetTile(const sf::Vector2i& pos, const std::string& type_id, const std::string& material_id);

        bool IsWall(const sf::Vector2i& pos) const;
        bool IsFloor(const sf::Vector2i& pos) const;
        bool BlocksSight(const sf::Vector2i& pos) const;

        //tile_state is a bitmask of Tile.hpp => TILE_STATE
        sf::Vector2i FindTile(const sf::Vector2i& pos, unsigned int tile_state) const;

        void Generate();

        //TODO: GetUnitAt, defriend Unit

        void AddLandmark(const std::string& id, const sf::Vector2i& pos);
        void RemoveLandmark(const std::string& id);
        sf::Vector2i GetLandmark(const std::string& id) const;

        Unit* PlaceUnit(const std::string& unit_type, const sf::Vector2i& pos, bool ignore_terrain = false);
        Unit* PlaceUnit(const std::string& unit_type, const std::string& landmark, bool ignore_terrain = false);

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
        void UpdateLightFields();

        sf::Color GetLightColorAt(const sf::Vector2i& pos) const;
};
#endif
