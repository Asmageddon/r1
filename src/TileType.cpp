#include "TileType.hpp"

TILE_CLASS TileType::get_tile_class(const std::string& type) {
    if (type == "wall")
        return TILE_WALL;
    if (type == "floor")
        return TILE_FLOOR;
    if (type == "void")
        return TILE_VOID;

    return TILE_UNKNOWN;
}

TileType::TileType() {
    id = "void";
    name = "void";
    desc = "";
    type = TILE_VOID;
    tileset = "tiles1.png";
    image = 0;
    cout << " * Loaded tiletype: void" << endl;
}
TileType::TileType(Data data) : Object(data) , Displayable(data) {
    type = get_tile_class(data.as_string("", "type"));
    std::cout << " * Loaded tiletype: " << id << std::endl;
}