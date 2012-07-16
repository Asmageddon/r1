#ifndef TILETYPE_HPP_
#define TILETYPE_HPP_

#include <string>
#include <iostream>

#include "Object.hpp"
#include "Displayable.hpp"

class Data;
class ResourceManager;

enum TILE_CLASS {
    TILE_UNKNOWN = 0,
    TILE_WALL,
    TILE_FLOOR,
    TILE_VOID
};

class TileType: public Object, public Displayable {
    public:
        ResourceManager *resman;
        TILE_CLASS type;
    public:
        static TILE_CLASS get_tile_class(const std::string& type);
        TileType() { /* WARNING: This does not initialize anything */ };
        //TODO: Make TileType and UnitType constructors const-correct, if possible
        TileType(ResourceManager *resman, Data data);
};

#endif