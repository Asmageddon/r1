#include "MapType.hpp"

#include "../Data.hpp"
#include "ResourceManager.hpp"
#include "Resource.hpp"

#include "../Tile.hpp"

#include "../utils.hpp"

#include "../Generators.hpp"

MapType::MapType(ResourceManager *resman, Data data) : Resource(data) {
    ambient = make_color(data["atmosphere"]["ambient"]);
    size = make_vector2u(data["terrain"]["size"]);

    generator_seed = data["terrain"]["seed"].as_int();

    std::set<AString> s = data["landmarks"].GetKeys();
    std::set<AString>::iterator it;
    for(it = s.begin(); it != s.end(); it++) {
        landmarks[*it] = make_vector2i(data["landmarks"][*it]);
    }
    landmarks[AString("center")] = sf::Vector2i(size.x / 2, size.y / 2);

    generator_tiles[AString("void")] = Tile(resman, "void", "void");

    //Just in case
    generator_tiles[AString("default")] = Tile(resman, "void", "void");

    s = data["terrain.tiles"].GetKeys();
    for(it = s.begin(); it != s.end(); it++) {
        std::vector<AString> sv = data["terrain.tiles"][*it].as_string_vector();
        Tile t = Tile(resman, sv[0], sv[1]);
        generator_tiles[*it] = t;
    }

    s = data.GetKeys("spawn.");
    for(it = s.begin(); it != s.end(); it++) {
        std::set<AString> subkeys = data[*it].GetKeys();
        std::set<AString>::const_iterator it2 = subkeys.begin();

        for(; it2 != subkeys.end(); it2++) {
            AString location = (*it).split(".")[1];

            std::vector<AString> vec = data[*it][*it2].split(" ");

            AString unit_type  = (*it2);
            int unit_count_min = 1;
            int unit_count_max = 1;

            if (vec.size() == 1) {
                unit_count_min = vec[0].as_int();
                unit_count_max = unit_count_min;
            }
            else if (vec.size() == 2) {
                unit_count_min = vec[0].as_int();
                unit_count_max = vec[1].as_int();
            }

            Spawn s;
            s.location = location;
            s.unit_id = unit_type;
            s.min_count = unit_count_min;
            s.max_count = unit_count_max;

            spawned_units[location][unit_type] = s;

        }
    }

    //WIP
    s = data.GetKeys("generator");
    generators.resize(s.size());
    unsigned int i;

    for((i = 0, it = s.begin()); it != s.end(); (it++, i++)) {
        AString k = (*it);
        AString type = data[k]["type"];
        if (type == "perlin") {
            generators[i] = new PerlinGenerator(*this, data, k);
        }
        else {
            generators[i] = NULL;
        }
    }
}
