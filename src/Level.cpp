#include "Level.hpp"

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

#include "LightField.hpp"

#include "Data.hpp"

Level::Level(const ResourceManager *resman, Data data) {
    this->resman = resman;

    id = data.as_string("", "id");
    ambient = data.as_Color("atmosphere", "ambient");

    size = data.as_Vector2u("terrain", "size");

    //TODO: Generator class, different types, seed, default_tile
    //generator = data.as_Vector2u("terrain", "generator");

    default_tile = Tile(resman, "void", "void");

    seed = data.as_int("terrain", "seed");
    if (seed == -1) {
        srand(time(NULL));
        seed = rand();
    }

    ready = false;
}

void Level::Create() {
    if (ready) {
        delete[] data;
    }

    data = new Tile[size.x * size.y];

    for(unsigned int x=0; x < size.x; x++)
    for(unsigned int y=0; y < size.y; y++) {
        data[x + size.x*y] = default_tile;
        data[x + size.x*y].integrity = 255;
        data[x + size.x*y].temperature = 127;
    }
    ready = true;
}

bool Level::IsReady() const {
    return ready;
}

const sf::Vector2u& Level::GetSize() const {
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

bool Level::IsWall(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return (t.type->type == TILE_WALL);
}

bool Level::IsFloor(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return (t.type->type == TILE_FLOOR);
}

bool Level::IsKnown(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.known;
}

bool Level::BlocksSight(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    if (t.type->blocks_sight)
        return true;
    if (t.unit != NULL)
        return t.unit->type->blocks_sight;

    return false;
}

void Level::Generate() {
    noise::module::Perlin perlin;
    perlin.SetOctaveCount (2);
    perlin.SetFrequency (1.0);

    Tile stone_wall  = Tile(resman, "generic_wall", "generic_stone");
    Tile stone_floor = Tile(resman, "generic_floor", "generic_stone");

    for(int x=0; x < (int)size.x; x++)
    for(int y=0; y < (int)size.y; y++) {
        double value = perlin.GetValue (0.14 * x, 0.14 * y, seed * 11.973);

        data[x + size.x*y] = stone_floor;
        data[x + size.x*y].integrity = 255;
        data[x + size.x*y].temperature = 127;

        if (value > 0.0)
            data[x + size.x*y] = stone_wall;
    }
}

Unit* Level::PlaceUnit(const std::string& unit_type, const sf::Vector2i& pos) {
    if (!InBounds(pos)) return NULL;
    if (GetTile(pos).unit != NULL) {
        Unit *tmp = GetTile(pos).unit;
        data[pos.x + size.x * pos.y].unit = NULL;
        units.erase(tmp);
        delete tmp;
    }

    Unit *u = new Unit(this->world, unit_type);
    u->SetLocation(this->id);
    u->SetPosition(pos);

    return u;
}

void Level::AttachLight(LightField *light) {
    lights.insert(light);
    light->Calculate(this, light->GetPosition());
}

void Level::DetachLight(LightField *light) {
    lights.erase(light);
}

void Level::UpdateLightFields() {
    std::set<LightField*>::iterator it = lights.begin();
    for(; it != lights.end(); it++) {
        (*it)->Recalculate();
    }
}

sf::Color Level::GetLightColorAt(const sf::Vector2i& pos) const {
    sf::Color result = ambient;

    std::set<LightField*>::iterator it = lights.begin();
    for (; it != lights.end(); it++) {
        result += (*it)->GetColorAt(pos);
    }

    return result;
}
