#ifndef TILE_HPP_
#define TILE_HPP_

#include "Unit.hpp"

#include <string>

class TileType;
class Material;
class ResourceManager;

enum TILE_STATE {
    IS_WALL = (1u << 0),
    IS_FLOOR = (1u << 1),
    NOT_WALL = (1u << 2),
    NOT_FLOOR = (1u << 3),
    TRANSPARENT = (1u << 4),
    OPAQUE = (1u << 5),
    HAS_UNIT = (1u << 6),
    NO_UNIT = (1u << 7),
    UNIT_TRANSPARENT = (1u << 8),
    UNIT_OPAQUE = (1u << 9),
    //Still unused: v
    HAS_ITEMS = (1u << 10),
    NO_ITEMS = (1u << 11),
    HAS_OBJECT = (1u << 12),
    NO_OBJECT = (1u << 13),
    OBJECT_TRANSPARENT = (1u << 14),
    OBJECT_OPAQUE = (1u << 15)
};

//TODO: Redo the vision system to work on what-is-visible-where basis (which will allow for portals and other neat stuff)
struct TileMemory {
    const TileType *type;
    const Material *material;
};

class Tile {
    public:
        const TileType *type;
        const Material *material;
        u_int8_t integrity;
        u_int8_t temperature;

        const ResourceManager *resman;

        TileMemory *last_known;

        Unit *unit;
    public:
        Tile() { /* WARNING: This doesn't initialize anything */ };
        Tile(const ResourceManager *resman, const std::string& type_id, const std::string& material_id);

        //TODO: Make this per-unit instead
        void SetKnown();
        void SetUnknown();
        //TODO: Setting and getting material and tiletype

        bool SatisfyAny(unsigned int tile_state) const;
        bool SatisfyAll(unsigned int tile_state) const;
};
#endif
