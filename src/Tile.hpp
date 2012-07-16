#ifndef TILE_HPP_
#define TILE_HPP_

#include "Unit.hpp"

#include <string>

class TileType;
class Material;
class ResourceManager;

class Tile {
    public:
        const TileType *type;
        const Material *material;
        u_int8_t integrity;
        u_int8_t temperature;

        ResourceManager *resman;

        //TODO: Known tiles should be saved as type/material instead of just known/unknown as they might be changed
        bool known;

        Unit *unit;
    public:
        Tile() { /* WARNING: This doesn't initialize anything */ };
        Tile(ResourceManager *resman, const std::string& type_id, const std::string& material_id);

        //TODO: Setting and getting material and tiletype
};
#endif