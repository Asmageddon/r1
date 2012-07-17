#include "TileType.hpp"

#include "Data.hpp"
#include "ResourceManager.hpp"

TILE_CLASS TileType::get_tile_class(const std::string& type) {
    if (type == "wall")
        return TILE_WALL;
    if (type == "floor")
        return TILE_FLOOR;
    if (type == "void")
        return TILE_VOID;

    return TILE_UNKNOWN;
}

TileType::TileType(ResourceManager *resman, Data data) : Object(data) , Displayable(data) {
    this->resman = resman;

    type = get_tile_class(data.as_string("", "type"));

    sprite = resman->GetSprite(tileset, image);
}
