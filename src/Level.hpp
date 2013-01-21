#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Resources.hpp"

//class ResourceManager;
#include "Tile.hpp"
class Unit;
class Data;

class World;

class LightField;

class Level {
    friend class Unit;
    friend class World;
    private:
        World *world;
        const ResourceManager *resman;

        const MapType *type;
        sf::Color ambient;

        Tile *data;
        AString id;
        int seed;

        bool ready;

        std::map<AString, sf::Vector2i> landmarks;

        std::set<Unit*> units;
        std::set<LightField*> lights;
    private:
        void SpawnUnits();
        void Create();
        void Generate();
    public:
        Level(World *world, const std::string& maptype_id);
        Level(World *world, const std::string& name, const std::string& maptype_id);

        bool IsReady() const;

        const sf::Vector2u& GetSize() const;

        bool InBounds(const sf::Vector2i& pos) const;

        void SetKnown(const sf::Vector2i& pos);
        const TileMemory *GetKnown(const sf::Vector2i& pos) const;
        bool IsKnown(const sf::Vector2i& pos) const;

        const Tile& GetTile(const sf::Vector2i& pos) const;
        //TODO: Add drawing functions
        void SetTile(const sf::Vector2i& pos, const std::string& type_id, const std::string& material_id);
        void SetTile(const sf::Vector2i& pos, const Tile& tile);

        bool IsWall(const sf::Vector2i& pos) const;
        bool IsFloor(const sf::Vector2i& pos) const;
        bool BlocksSight(const sf::Vector2i& pos) const;

        //tile_state is a bitmask of TILE_STATE enum members (defined in Tile.hpp)
        sf::Vector2i FindTile(const sf::Vector2i& pos, unsigned int tile_state) const;

        //TODO: GetUnitAt, defriend Unit

        void AddLandmark(const std::string& id, const sf::Vector2i& pos);
        void RemoveLandmark(const std::string& id);
        sf::Vector2i GetLandmark(const std::string& id) const;

        Unit* PlaceUnit(const std::string& unit_type, const sf::Vector2i& pos, bool ignore_terrain = false);
        Unit* PlaceUnit(const std::string& unit_type, const std::string& landmark, bool ignore_terrain = false);

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
        void UpdateLightFields();

        void RegisterUnit(Unit *unit);
        void DeregisterUnit(Unit *unit);

        //TODO: Make this faster than O(N) - cache the lightmap or have an array of (larger) integers from which I substract and add
        sf::Color GetLightColorAt(const sf::Vector2i& pos) const;

        int Simulate(Unit *reference_unit); //Please don't call this manually; TODO: Make this private and friend World ^^

        const sf::Color& GetAmbientColor() const;
        void SetAmbientColor(const sf::Color& color);

        //To be used by generators
        inline void FastSetTile(unsigned int x, unsigned int y, const Tile& t) {
            data[x + type->size.x * y] = t;
        }

        const MapType* GetMapType() const;

        int GetSeed() const;
};
#endif
