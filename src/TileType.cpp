#include "TileType.hpp"

TILE_CLASS TileType::get_tile_class(string type) {
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
    type = get_tile_class(data[""]["type"]);
    cout << " * Loaded tiletype: " << id << endl;
}