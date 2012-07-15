#include "Level.hpp"

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

Level::Level(ResourceManager* resman, const Vector2u& size) {
    this->resman = resman;
    data = new Tile[size.x * size.y];
    this->size = size;
    Generate();
    default_tile = Tile();
}

const Vector2u& Level::GetSize() const {
    return size;
}

bool Level::InBounds(const sf::Vector2i& pos) const {
    if      (pos.x < 0) return false;
    else if (pos.x >= (int)size.x) return false;
    else if (pos.y < 0) return false;
    else if (pos.y >= (int)size.y) return false;
    return true;
}

void Level::SetKnown(const sf::Vector2i& pos, const bool& state) {
    if (InBounds(pos))
        data[pos.x + size.x * pos.y].known = state;
}

const Tile& Level::GetTile(const sf::Vector2i& pos) const {
    if (!InBounds(pos))
        return default_tile;
    return data[pos.x + size.x * pos.y];
}

void Level::SetTile(const sf::Vector2i& pos, const Tile& new_tile) {
    data[pos.x + size.x * pos.y] = new_tile;
}

bool Level::IsWall(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return (t.type == TILE_WALL);
}

bool Level::IsFloor(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return (t.type == TILE_FLOOR);
}

bool Level::IsKnown(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.known;
}

void Level::SetDefaultTile(const Tile& tile) {
    default_tile = tile;
}

void Level::Generate() {
    noise::module::Perlin perlin;
    perlin.SetOctaveCount (2);
    perlin.SetFrequency (1.0);

    for(int x=0; x < (int)size.x; x++)
    for(int y=0; y < (int)size.y; y++) {
        double value = perlin.GetValue (0.14 * x, 0.14 * y, 0.5);

        data[x + size.x*y].type = 2;
        data[x + size.x*y].material = 1;
        data[x + size.x*y].integrity = 255;
        data[x + size.x*y].temperature = 127;

        if (value > 0.0)
            data[x + size.x*y].type = 1;
    }
    Unit *u = new Unit();
    u->type = 1;
    PlaceUnit(sf::Vector2i(12,5), u);
    player = u;

    u = new Unit();
    u->type = 2;
    u->material = 1;
    PlaceUnit(sf::Vector2i(12,3), u);
}

void Level::PlaceUnit(const sf::Vector2i& pos, Unit* unit) {
    if (data[pos.x + size.x * pos.y].unit != NULL) {
        units.erase(data[pos.x + size.x * pos.y].unit);
        delete data[pos.x + size.x * pos.y].unit;
    }
    data[pos.x + size.x * pos.y].unit = unit;
    units[unit] = pos;
    unit->pos = pos;
}

void Level::MoveUnit(Unit* unit, const sf::Vector2i& vec) {
    sf::Vector2i new_pos = unit->pos + vec;

    if (!InBounds(new_pos)) return;

    Unit* tmp = NULL;
    sf::Vector2i old_pos = unit->pos;
    if (data[new_pos.x + size.x * new_pos.y].unit != NULL) {
        tmp = data[new_pos.x + size.x * new_pos.y].unit;
        tmp->pos = unit->pos;
    }
    unit->pos = new_pos;
    data[new_pos.x + size.x * new_pos.y].unit = unit;
    units[unit] = new_pos;

    data[old_pos.x + size.x * old_pos.y].unit = NULL;
    if (tmp != NULL) {
        data[old_pos.x + size.x * old_pos.y].unit = tmp;
        units[tmp] = old_pos;
    }
}