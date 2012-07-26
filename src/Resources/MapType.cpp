#include "MapType.hpp"

#include "../Data.hpp"
#include "ResourceManager.hpp"
#include "Resource.hpp"

#include "../Tile.hpp"

MapType::MapType(ResourceManager *resman, Data data) : Resource(data) {
    ambient = data.as_Color("atmosphere", "ambient");
    size = data.as_Vector2u("terrain", "size");

    default_tile = Tile(resman, "void", "void"); //<<<<<<<<

    generator_seed = data.as_int("terrain", "seed");

    std::set<AString> s = data.GetKeys("landmarks", "");
    std::set<AString>::iterator it;
    for(it = s.begin(); it != s.end(); it++) {
        landmarks[*it] = data.as_Vector2i("landmarks", *it);
    }
    landmarks[AString("center")] = sf::Vector2i(size.x / 2, size.y / 2);

    s = data.GetKeys("terrain.tiles", "");
    for(it = s.begin(); it != s.end(); it++) {
        std::vector<AString> sv = data.as_str_vector("terrain.tiles", *it);
        Tile t = Tile(resman, sv[0], sv[1]);
        generator_tiles[*it] = t;
    }

    //TODO: Spawns
}
