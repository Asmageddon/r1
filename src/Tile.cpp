#include "Tile.hpp"

#include "Unit.hpp"

#include "AString.hpp"

#include "Resources.hpp"

//class TileType;
//class Material;

Tile::Tile(const ResourceManager *resman, const std::string& type_id, const std::string& material_id) {
    type = &(resman->GetTileType(type_id));
    material = &(resman->GetMaterial(material_id));

    integrity = 0;
    temperature = 0;
    unit = NULL;

    last_known = NULL;
}

bool Tile::operator==(const Tile& rhs) const {
    //This should be fine since there will never be multiple instances of the same material/tiletype
    if (material != rhs.material) return false;
    if (type != rhs.type) return false;

    return true;
}

bool Tile::operator!=(const Tile& rhs) const {
    return !((*this) == rhs);
}

bool Tile::operator<(const Tile& rhs) const {
    if (material < rhs.material) return true;
    if (material > rhs.material) return false;

    if (type < rhs.type) return true;
    if (type > rhs.type) return false;

    return false;
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


bool Tile::SatisfyAny(unsigned int tile_state) const {
    if ((tile_state & IS_WALL) && (type->type == TILE_WALL)) return true;
    if ((tile_state & IS_FLOOR) && (type->type == TILE_FLOOR)) return true;

    if ((tile_state & NOT_WALL) && (type->type != TILE_WALL)) return true;
    if ((tile_state & NOT_FLOOR) && (type->type != TILE_FLOOR)) return true;

    if ((tile_state & TRANSPARENT) && (!type->blocks_sight)) return true;
    if ((tile_state & OPAQUE) && (type->blocks_sight)) return true;

    if ((tile_state & HAS_UNIT) && (unit != NULL)) return true;
    if ((tile_state & NO_UNIT) && (unit == NULL)) return true;

    if ((tile_state & UNIT_TRANSPARENT) && (unit != NULL)){
        if (!unit->type->blocks_sight) return true;
    }
    if ((tile_state & UNIT_OPAQUE) && (unit != NULL)){
        if (unit->type->blocks_sight) return true;
    }

    return false;
}

bool Tile::SatisfyAll(unsigned int tile_state) const {
    if ((tile_state & IS_WALL) && (type->type != TILE_WALL)) return false;
    if ((tile_state & IS_FLOOR) && (type->type != TILE_FLOOR)) return false;

    if ((tile_state & NOT_WALL) && (type->type == TILE_WALL)) return false;
    if ((tile_state & NOT_FLOOR) && (type->type == TILE_FLOOR)) return false;

    if ((tile_state & TRANSPARENT) && (type->blocks_sight)) return false;
    if ((tile_state & OPAQUE) && (!type->blocks_sight)) return false;

    if ((tile_state & HAS_UNIT) && (unit == NULL)) return false;
    if ((tile_state & NO_UNIT) && (unit != NULL)) return false;

    if ((tile_state & UNIT_TRANSPARENT) && (unit != NULL)){
        if (unit->type->blocks_sight) return false;
    }
    if ((tile_state & UNIT_OPAQUE) && (unit != NULL)){
        if (!unit->type->blocks_sight) return false;
    }

    return true;
}
