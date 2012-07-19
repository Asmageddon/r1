#include "Tile.hpp"

#include "Unit.hpp"

#include <string>

#include "ResourceManager.hpp"

class TileType;
class Material;

Tile::Tile(const ResourceManager *resman, const std::string& type_id, const std::string& material_id) {
    type = &(resman->GetTileType(type_id));
    material = &(resman->GetMaterial(material_id));

    integrity = 0;
    temperature = 0;
    unit = NULL;

    last_known = NULL;
}

void Tile::SetKnown() {
    if (last_known == NULL)
        last_known = new TileMemory();

    last_known->material = this->material;
    last_known->type = this->type;
}
void Tile::SetUnknown() {
    if (last_known != NULL)
        delete last_known;
}

//enum TILE_STATE {
    //IS_WALL = 1,
    //IS_FLOOR = 2,
    //IS_VOID = 4,
    //HAS_UNIT = 8,
    //UNIT_TRANSPARENT = 16,
    //UNIT_OPAQUE = 32,
    ////Still unused: v
    //HAS_ITEMS = 64,
    //HAS_OBJECT = 128,
    //OBJECT_TRANSPARENT = 256,
    //OBJECT_OPAQUE = 512,
//};


bool Tile::MatchAny(unsigned int tile_state) const {
    if ((tile_state & IS_WALL) && (type->type == TILE_WALL)) return true;
    if ((tile_state & IS_FLOOR) && (type->type == TILE_FLOOR)) return true;
    if ((tile_state & IS_VOID) && (type->type == TILE_VOID)) return true;

    if ((tile_state & TRANSPARENT) && (!type->blocks_sight)) return true;
    if ((tile_state & OPAQUE) && (type->blocks_sight)) return true;

    if ((tile_state & HAS_UNIT) && (unit != NULL)) return true;
    if ((tile_state & UNIT_TRANSPARENT) && (unit != NULL)){
        if (!unit->type->blocks_sight) return true;
    }
    if ((tile_state & UNIT_OPAQUE) && (unit != NULL)){
        if (unit->type->blocks_sight) return true;
    }

    return false;
}

bool Tile::MatchAll(unsigned int tile_state) const {
    if ((tile_state & IS_WALL) && (type->type != TILE_WALL)) return false;
    if ((tile_state & IS_FLOOR) && (type->type != TILE_FLOOR)) return false;
    if ((tile_state & IS_VOID) && (type->type != TILE_VOID)) return false;

    if ((tile_state & TRANSPARENT) && (type->blocks_sight)) return false;
    if ((tile_state & OPAQUE) && (!type->blocks_sight)) return false;

    if ((tile_state & HAS_UNIT) && (unit == NULL)) return false;
    if ((tile_state & UNIT_TRANSPARENT) && (unit != NULL)){
        if (unit->type->blocks_sight) return false;
    }
    if ((tile_state & UNIT_OPAQUE) && (unit != NULL)){
        if (!unit->type->blocks_sight) return false;
    }

    return true;
}
