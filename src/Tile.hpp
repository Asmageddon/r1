#ifndef TILE_HPP_
#define TILE_HPP_

#include "Unit.hpp"

class Tile {
    public:
        u_int16_t type;
        u_int16_t material;
        u_int8_t integrity;
        u_int8_t temperature;

        bool known;

        Unit *unit;
    public:
        Tile() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;
            unit = NULL;

            known = false;
        }
};
#endif