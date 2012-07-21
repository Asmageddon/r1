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

    //If it's not set in the data file, set it basing on type
    if (!data.HasField("appearance", "blocks_sight")) {
        if (type == TILE_WALL)
            blocks_sight = true;
    }

    //TODO: Add borders for different kinds of neighbouring tiles
    border_tileset = data.as_string("appearance", "border");
    if (border_tileset == "")
        border_sprite = NULL;
    else {
        border_sprite = new TileSprite;
        (*border_sprite) = resman->GetSprite(border_tileset, 0);
    }

    sprite = resman->GetSprite(tileset, image);
}
