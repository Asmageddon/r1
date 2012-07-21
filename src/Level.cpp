#include "Level.hpp"

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

#include "LightField.hpp"

#include "World.hpp"

#include "Data.hpp"

Level::Level(World *world, Data data) {
    this->world = world;
    this->resman = world->GetResman();

    id = data.as_string("", "id");
    ambient = data.as_Color("atmosphere", "ambient");

    size = data.as_Vector2u("terrain", "size");

    //TODO: Generator class, different types, seed, default_tile
    //generator = data.as_Vector2u("terrain", "generator");

    default_tile = Tile(resman, "void", "void");

    seed = data.as_int("terrain", "seed");
    if (seed == -1) {
        //srand(time(NULL));
        seed = rand();
    }

    //TODO: Automatic landmarks - center, random, etc.
    std::set<std::string> s = data.GetKeys("landmarks", "");

    std::set<std::string>::iterator it;

    for(it = s.begin(); it != s.end(); it++) {
        landmarks[*it] = data.as_Vector2i("landmarks", *it);
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

void Level::SetKnown(const sf::Vector2i& pos) {
    if (InBounds(pos))
        data[pos.x + size.x * pos.y].SetKnown();
}
const TileMemory* Level::GetKnown(const sf::Vector2i& pos) const {
    static TileMemory empty_memory = { default_tile.type, default_tile.material };

    if (InBounds(pos))
        return data[pos.x + size.x * pos.y].last_known;

    return &empty_memory;
}

bool Level::IsKnown(const sf::Vector2i& pos) const {
    if (!InBounds(pos))
        return false;

    return data[pos.x + size.x * pos.y].last_known != NULL;
}

//TODO: Make this return a non-const pointer (after sanitizing Tile api ofc)
const Tile& Level::GetTile(const sf::Vector2i& pos) const {
    if (!InBounds(pos))
        return default_tile;
    return data[pos.x + size.x * pos.y];
}

bool Level::IsWall(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.MatchAny(IS_WALL);
}

bool Level::IsFloor(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.MatchAny(IS_FLOOR);
}

bool Level::BlocksSight(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.MatchAny(OPAQUE | UNIT_OPAQUE | OBJECT_OPAQUE);
}

sf::Vector2i Level::FindTile(const sf::Vector2i& pos, unsigned int tile_state) const {
    //Searches in rhomboid shape
    if (GetTile(pos).MatchAll(tile_state)) return pos;

    sf::Vector2i _pos;
    int x = pos.x;
    int y = pos.y;

    int dist = 1;
    int step = 0;
    enum { UP, RIGHT, DOWN, LEFT } direction = UP;
    while (true) {
        switch(direction) {
            case UP:
                y = pos.y - dist + step;
                x = pos.x + step;
                _pos = sf::Vector2i(x, y);
                step++;
                if (step == dist) { direction = RIGHT; step = 0; }
                break;
            case RIGHT:
                y = pos.y + step;
                x = pos.x + dist - step;
                _pos = sf::Vector2i(x, y);
                step++;
                if (step == dist) { direction = DOWN; step = 0; }
                break;
            case DOWN:
                y = pos.y + dist - step;
                x = pos.x - step;
                _pos = sf::Vector2i(x, y);
                step++;
                if (step == dist) { direction = LEFT; step = 0; }
                break;
            case LEFT:
                y = pos.y - step;
                x = pos.x - dist + step;
                _pos = sf::Vector2i(x, y);
                step++;
                if (step == dist) { direction = UP; step = 0; dist++; }
                break;
        }
        if (GetTile(_pos).MatchAll(tile_state)) return _pos;

        //If we can't find a free spot, just return whatever
        if (dist > 16) return _pos;
    }
}

void Level::Generate() {
    noise::module::Perlin perlin;
    perlin.SetOctaveCount (2);
    perlin.SetFrequency (1.0);

    //TODO: Get materials from data files
    //TODO: Separate classes for generator(s)
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

void Level::AddLandmark(const std::string& id, const sf::Vector2i& pos) {
    sf::Vector2i _pos = pos;
    if      (_pos.x < 0) _pos.x = 0;
    else if (_pos.x >= (int)size.x) _pos.x = size.x;
    else if (_pos.y < 0) _pos.y = 0;
    else if (_pos.y >= (int)size.y) _pos.y = size.y;

    landmarks[id] = _pos;
}
void Level::RemoveLandmark(const std::string& id) {
    landmarks.erase(id);
}
sf::Vector2i Level::GetLandmark(const std::string& id) const {
    std::string _id = id;
    if (id == "") _id = "default";

    sf::Vector2i pos;

    //TODO: Make this return raw pos, find free in PlaceUnit, etc.
    if (id == "random") {
        //TODO: Replace this with perlin-noise based pseudorandom numbers
        int x = rand() % size.x;
        int y = rand() % size.y;
        return sf::Vector2i(x, y);
    }
    if (contains(landmarks, _id))
        return const_access(landmarks, _id);

    return const_access(landmarks, "default");
}

Unit* Level::PlaceUnit(const std::string& unit_type, const sf::Vector2i& pos, bool ignore_terrain) {
    sf::Vector2i _pos;
    if (ignore_terrain)
        _pos = pos;
    else
        _pos = FindTile(pos, IS_FLOOR | NO_UNIT);

    if (GetTile(_pos).unit != NULL) {
        Unit *tmp = GetTile(_pos).unit;
        data[_pos.x + size.x * _pos.y].unit = NULL;
        units.erase(tmp);
        delete tmp;
    }

    Unit *u = new Unit(this->world, unit_type);
    u->SetLocation(this->id);
    u->SetPosition(_pos);

    return u;
}

Unit* Level::PlaceUnit(const std::string& unit_type, const std::string& landmark, bool ignore_terrain) {
    return PlaceUnit(unit_type, GetLandmark(landmark), ignore_terrain);
}

void Level::AttachLight(LightField *light) {
    lights.insert(light);
    //light->Calculate(this, light->GetPosition());
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
