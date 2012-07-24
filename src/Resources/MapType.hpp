#ifndef MAPTYPE_HPP_
#define MAPTYPE_HPP_

#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "Resource.hpp"

#include "../Tile.hpp"

class Data;
class ResourceManager;

struct Spawn {
    std::string unit_id;
    unsigned int min_count;
    unsigned int max_count;
};

class MapType : public Resource {
    public:
        sf::Color ambient;
        sf::Vector2u size;
        std::string generator;
        int generator_seed;
        Tile default_tile;
        std::map<std::string, Tile> generator_tiles;
        std::map<std::string, sf::Vector2i> landmarks;
        std::map<std::string, Spawn> spawned_units;
    public:
        MapType() { /* WARNING: This does not initialize anything */ };
        MapType(ResourceManager *resman, Data data);
};

#endif
