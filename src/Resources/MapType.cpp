#include "MapType.hpp"

#include "../Data.hpp"
#include "ResourceManager.hpp"
#include "Resource.hpp"

#include "../Tile.hpp"

#include "../utils.hpp"

MapType::MapType(ResourceManager *resman, Data data) : Resource(data) {
    ambient = make_color(data["atmosphere"]["ambient"]);
    size = make_vector2u(data["terrain"]["size"]);

    default_tile = Tile(resman, "void", "void"); //<<<<<<<<

    generator_seed = data["terrain"]["seed"].as_int();

    std::set<AString> s = data["landmarks"].GetKeys();
    std::set<AString>::iterator it;
    for(it = s.begin(); it != s.end(); it++) {
        landmarks[*it] = make_vector2i(data["landmarks"][*it]);
    }
    landmarks[AString("center")] = sf::Vector2i(size.x / 2, size.y / 2);

    s = data["terrain.tiles"].GetKeys();
    for(it = s.begin(); it != s.end(); it++) {
        std::vector<AString> sv = data["terrain.tiles"][*it].as_string_vector();
        Tile t = Tile(resman, sv[0], sv[1]);
        generator_tiles[*it] = t;
    }

    //WIP: Spawns
    //s = data.GetKeys("spawn", "");
    //for(it = s.begin(); it != s.end(); it++) {
        //std::vector<AString> sv = data.as_str_vector("terrain.tiles", *it);
        //Tile t = Tile(resman, sv[0], sv[1]);
        //generator_tiles[*it] = t;
    //}
}
