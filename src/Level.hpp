#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

class Level {
    private:
        ResourceManager* resman;
        Tile *data;
        Tile default_tile;
        Vector2u size;
    public:
        std::map<Unit*, sf::Vector2i> units;
        Unit *player;
        Color ambient;
    public:
        Level(ResourceManager* resman, const Vector2u& size);

        const Vector2u& GetSize() const;

        bool InBounds(const sf::Vector2i& pos) const;

        void SetKnown(const sf::Vector2i& pos, const bool& state);

        const Tile& GetTile(const sf::Vector2i& pos) const;

        void SetTile(const sf::Vector2i& pos, const Tile& new_tile);

        bool IsWall(const sf::Vector2i& pos) const;

        bool IsFloor(const sf::Vector2i& pos) const;

        bool IsKnown(const sf::Vector2i& pos) const;

        void SetDefaultTile(const Tile& tile);

        void Generate();
        void PlaceUnit(const sf::Vector2i& pos, Unit* unit);
        void MoveUnit(Unit* unit, const sf::Vector2i& vec);
};
#endif