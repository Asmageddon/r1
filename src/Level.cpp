#include "Level.hpp"

#include <set>
#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Unit.hpp"

#include "LightField.hpp"

Level::Level(ResourceManager* resman, const sf::Vector2u& size) {
    this->resman = resman;

    data = new Tile[size.x * size.y];
    this->size = size;

    SetDefaultTile(Tile(resman, "void", "void"));
    Generate();
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

void Level::SetDefaultTile(const Tile& tile) {
    default_tile = tile;
}

void Level::Generate() {
    noise::module::Perlin perlin;
    perlin.SetOctaveCount (2);
    perlin.SetFrequency (1.0);

    Tile stone_wall  = Tile(resman, "generic_wall", "generic_stone");
    Tile stone_floor = Tile(resman, "generic_floor", "generic_stone");

    for(int x=0; x < (int)size.x; x++)
    for(int y=0; y < (int)size.y; y++) {
        double value = perlin.GetValue (0.14 * x, 0.14 * y, 0.5);

        data[x + size.x*y] = stone_floor;
        data[x + size.x*y].integrity = 255;
        data[x + size.x*y].temperature = 127;

        if (value > 0.0)
            data[x + size.x*y] = stone_wall;
    }
    Unit *u = new Unit(resman, "test_player");
    u->SetLocation(this);
    u->SetPosition(sf::Vector2i(12,5));
    player = u;

    u = new Unit(resman, "golem");
    u->SetLocation(this);
    u->SetPosition(sf::Vector2i(12,3));

    u = new Unit(resman, "sun_sentry");
    u->SetLocation(this);
    u->SetPosition(sf::Vector2i(26, 25));
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