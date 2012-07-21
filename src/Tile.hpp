#ifndef TILE_HPP_
#define TILE_HPP_

#include "Unit.hpp"

#include <string>

class TileType;
class Material;
class ResourceManager;

enum TILE_STATE {
    IS_WALL = 1,
    IS_FLOOR = 2,
    IS_VOID = 4,
    TRANSPARENT = 8,
    OPAQUE = 16,
    HAS_UNIT = 32,
    NO_UNIT = 64,
    UNIT_TRANSPARENT = 128,
    UNIT_OPAQUE = 256,
    //Still unused: v
    HAS_ITEMS = 512,
    NO_ITEMS = 1024,
    HAS_OBJECT = 2048,
    NO_OBJECT = 4096,
    OBJECT_TRANSPARENT = 8192,
    OBJECT_OPAQUE = 16384
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

        bool MatchAny(unsigned int tile_state) const;
        bool MatchAll(unsigned int tile_state) const;
};
#endif
