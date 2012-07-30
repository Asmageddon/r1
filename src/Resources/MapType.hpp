#ifndef MAPTYPE_HPP_
#define MAPTYPE_HPP_

#include "../AString.hpp"
#include <map>

#include <SFML/Graphics.hpp>

#include "Resource.hpp"

#include "../Tile.hpp"

class Data;
class ResourceManager;

struct Spawn {
    AString unit_id;
    AString location;
    unsigned int min_count;
    unsigned int max_count;
};

class MapType : public Resource {
    public:
        sf::Color ambient;
        sf::Vector2u size;
        AString generator;
        int generator_seed;
        Tile default_tile;
        std::map<AString, Tile> generator_tiles;
        std::map<AString, sf::Vector2i> landmarks;
        //landmark => (type => Spawn)
        std::map<AString, std::map<AString, Spawn> > spawned_units;
    public:
        MapType() { /* WARNING: This does not initialize anything */ };
        MapType(ResourceManager *resman, Data data);
};

#endif
