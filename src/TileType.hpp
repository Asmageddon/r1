#ifndef TILETYPE_HPP_
#define TILETYPE_HPP_

#include <string>
#include <iostream>

#include "Object.hpp"
#include "Displayable.hpp"
#include "Data.hpp"

enum TILE_CLASS {
    TILE_UNKNOWN = 0,
    TILE_WALL,
    TILE_FLOOR,
    TILE_VOID
};

class TileType: public Object, public Displayable {
    public:
        TILE_CLASS type;
    public:
        static TILE_CLASS get_tile_class(const std::string& type);
        TileType();
        TileType(Data data);
};

#endif