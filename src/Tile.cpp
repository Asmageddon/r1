#include "Tile.hpp"

#include "Unit.hpp"

#include <string>

#include "ResourceManager.hpp"

class TileType;
class Material;

Tile::Tile(ResourceManager *resman, const std::string& type_id, const std::string& material_id) {
    type = &(resman->GetTileType(type_id));
    material = &(resman->GetMaterial(material_id));

    integrity = 0;
    temperature = 0;
    unit = NULL;

    known = false;
}