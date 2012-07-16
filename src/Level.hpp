#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

class LightField;

class Level {
    friend class Unit;
    private:
        ResourceManager* resman;
        Tile *data;
        Tile default_tile;
        sf::Vector2u size;
    public:
        std::set<Unit*> units;
        std::set<LightField*> lights;
        Unit *player;
        sf::Color ambient;
    public:
        Level(ResourceManager* resman, const sf::Vector2u& size);

        const sf::Vector2u& GetSize() const;

        bool InBounds(const sf::Vector2i& pos) const;

        void SetKnown(const sf::Vector2i& pos, const bool& state);

        const Tile& GetTile(const sf::Vector2i& pos) const;

        void SetTile(const sf::Vector2i& pos, const Tile& new_tile);

        bool IsWall(const sf::Vector2i& pos) const;

        bool IsFloor(const sf::Vector2i& pos) const;

        bool IsKnown(const sf::Vector2i& pos) const;

        void SetDefaultTile(const Tile& tile);

        void Generate();

        void AttachLight(LightField *light);
        void DetachLight(LightField *light);
        void UpdateLightFields();

        sf::Color GetLightColorAt(const sf::Vector2i& pos) const;
};
#endif