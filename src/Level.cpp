#include "Level.hpp"

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Resources.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

#include "LightField.hpp"
#include "SightField.hpp"

#include "World.hpp"

#include "utils.hpp"

#include "Generators.hpp"

Level::Level(World *world, const std::string& maptype_id) : world(world) {
    resman = world->GetResman();

    type = &(resman->GetMapType(maptype_id));
    id = maptype_id + "_instance"; //TODO: Get a random (or non-random) ID from ResourceManager; Implement IDs for resource instances
    landmarks = type->landmarks;
    ambient = type->ambient;

    seed = type->generator_seed;
    if (seed == -1) { seed = rand(); }

    std::cout << "Seed = " << seed << std::endl;

    ready = false;
}

Level::Level(World *world, const std::string& name, const std::string& maptype_id) : world(world) {
    resman = world->GetResman();

    type = &(resman->GetMapType(maptype_id));
    id = name;
    landmarks = type->landmarks;
    ambient = type->ambient;

    seed = type->generator_seed;
    if (seed == -1) { seed = rand(); }

    std::cout << "Seed = " << seed << std::endl;

    ready = false;
}

void Level::Create() {
    if (ready) {
        delete[] data;
    }

    const sf::Vector2u& size = type->size;

    data = new Tile[size.x * size.y];

    for(unsigned int x=0; x < size.x; x++)
    for(unsigned int y=0; y < size.y; y++) {
        data[x + size.x*y] = const_access(type->generator_tiles, "default");
        data[x + size.x*y].integrity = 255;
        data[x + size.x*y].temperature = 127;
    }
    ready = true;
}

void Level::SpawnUnits() {
    std::map<AString, std::map<AString, Spawn> >::const_iterator it = type->spawned_units.begin();

    for(; it != type->spawned_units.end(); it++) {
        const std::map<AString, Spawn>& submap = it->second;

        std::map<AString, Spawn>::const_iterator it2 = submap.begin();

        for(; it2 != submap.end(); it2++) {
            Spawn s = it2->second;
            int c;
            if (s.max_count == s.min_count)
                c = s.min_count;
            else
                c = rand() % (s.max_count - s.min_count) + s.min_count;

            for(int i = 0; i < c; i++) {
                Unit *u = PlaceUnit(it2->first, it->first);

                if (u->type->ai == "player")
                    world->player = u;
            }
        }
    }
}

bool Level::IsReady() const {
    return ready;
}

const sf::Vector2u& Level::GetSize() const {
    return type->size;
}

bool Level::InBounds(const sf::Vector2i& pos) const {
    if      (pos.x < 0) return false;
    else if (pos.x >= (int)type->size.x) return false;
    else if (pos.y < 0) return false;
    else if (pos.y >= (int)type->size.y) return false;
    return true;
}

void Level::SetKnown(const sf::Vector2i& pos) {
    if (InBounds(pos))
        data[pos.x + type->size.x * pos.y].SetKnown();
}
const TileMemory* Level::GetKnown(const sf::Vector2i& pos) const {
    static TileMemory empty_memory = {
        const_access(type->generator_tiles, "default").type,
        const_access(type->generator_tiles, "default").material
    };

    if (InBounds(pos))
        return data[pos.x + type->size.x * pos.y].last_known;

    return &empty_memory;
}

bool Level::IsKnown(const sf::Vector2i& pos) const {
    if (!InBounds(pos))
        return false;

    return data[pos.x + type->size.x * pos.y].last_known != NULL;
}

const Tile& Level::GetTile(const sf::Vector2i& pos) const {
    if (!InBounds(pos))
        return const_access(type->generator_tiles, "default");
    return data[pos.x + type->size.x * pos.y];
}

void Level::SetTile(const sf::Vector2i& pos, const std::string& type_id, const std::string& material_id) {
    Tile new_tile  = Tile(resman, type_id, material_id);

    SetTile(pos, new_tile);
}

void Level::SetTile(const sf::Vector2i& pos, const Tile& new_tile) {
    if (!InBounds(pos)) return;

    unsigned int i = pos.x + type->size.x * pos.y;

    Tile prev_tile = data[i];

    data[i] = new_tile;
    data[i].unit = prev_tile.unit;
    data[i].last_known = prev_tile.last_known;

    std::set<LightField*>::iterator lit = lights.begin();
    for(; lit != lights.end(); lit++) {
        if ((*lit)->GetIntensityAt(pos) > 0.0f)
            (*lit)->Recalculate();
    }
    std::set<Unit*>::iterator uit = units.begin();
    for(; uit != units.end(); uit++) {
        if ((*uit)->CanSee(pos)) {
            (*uit)->RecalculateFOV();
        }
    }

}

bool Level::IsWall(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.SatisfyAny(IS_WALL);
}

bool Level::IsFloor(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.SatisfyAny(IS_FLOOR);
}

bool Level::BlocksSight(const sf::Vector2i& pos) const {
    const Tile& t = GetTile(pos);
    return t.SatisfyAny(OPAQUE | UNIT_OPAQUE | OBJECT_OPAQUE);
}

sf::Vector2i Level::FindTile(const sf::Vector2i& pos, unsigned int tile_state) const {
    //Searches in rhomboid shape
    if (GetTile(pos).SatisfyAll(tile_state)) return pos;

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
        if (GetTile(_pos).SatisfyAll(tile_state)) return _pos;

        //If we can't find a free spot, just return initial position
        if (dist > 16) return pos;
    }
}

void Level::Generate() {
    std::vector<Generator*>::const_iterator it = type->generators.begin();

    for(; it != type->generators.end(); it++) {
        if ((*it) != NULL) {
            (*it)->GenerateLevel(*this);
        }
    }

    SpawnUnits();
}

void Level::AddLandmark(const std::string& id, const sf::Vector2i& pos) {
    sf::Vector2i _pos = pos;
    const sf::Vector2u& size = type->size;

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
    AString _id = id;
    if (id == "") _id = "default";

    const sf::Vector2u& size = type->size;

    sf::Vector2i pos;

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
        data[_pos.x + type->size.x * _pos.y].unit = NULL;
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

void Level::RegisterUnit(Unit *unit) {
    units.insert(unit);
}

void Level::DeregisterUnit(Unit *unit) {
    units.erase(unit);
}

sf::Color Level::GetLightColorAt(const sf::Vector2i& pos) const {
    sf::Color result = ambient;

    std::set<LightField*>::iterator it = lights.begin();
    for (; it != lights.end(); it++) {
        result += (*it)->GetColorAt(pos);
    }

    return result;
}

int Level::Simulate(Unit *reference_unit) {
    //TODO: Limit number of steps in one call so the player can see what is happening around him while waiting or something
    if (reference_unit->GetCurrentLevel() != this) return 0;

    unsigned int i = 0;

    unsigned int min_ticks = world->GetTicksPerFrameMin();
    unsigned int max_ticks = world->GetTicksPerFrameMax();

    while ((reference_unit->GetNextAction() != NULL) | (i < min_ticks)) {
        if (++i > max_ticks) break; //TODO: Make this into a variable and/or a parameter

        std::set<Unit*>::iterator it = units.begin();
        for(; it != units.end(); it++) {
            (*it)->Simulate();
        }
    }

    return i;
}


const sf::Color& Level::GetAmbientColor() const {
    return ambient;
}
void Level::SetAmbientColor(const sf::Color& color) {
    ambient = color;
}
const MapType* Level::GetMapType() const {
    return type;
}

int Level::GetSeed() const {
    return seed;
}
