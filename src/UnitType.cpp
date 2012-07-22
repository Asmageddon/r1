#include "UnitType.hpp"

#include <string>
#include <vector>

#include "Object.hpp"
#include "Displayable.hpp"
#include "Glowable.hpp"

#include "Tile.hpp"

#include "TileSprite.hpp"

#include "ResourceManager.hpp"

UnitType::UnitType(ResourceManager *resman, Data data) : Object(data), Displayable(data), Glowable(data) {
    this->resman = resman;

    categories = data.as_str_vector("", "categories");

    material = data.as_string("body", "material");

    sprite = resman->GetSprite(tileset, image);

    sight_radius = data.as_int("stats", "sight.radius");

    if (data.HasField("stats", "sight.vision_tint")) {
        sight_threshold = data.as_float("stats", "sight.threshold");
    }
    else {
        sight_threshold = 0.33;
    }

    travel_conditions = 0;

    if (data.HasField("stats", "travel.walls")) {
        travel_walls = data.as_bool("stats", "travel.walls");
        if (!travel_walls)
            travel_conditions |= NOT_WALL;
    }
    else {
        travel_walls = false;
        travel_conditions |= NOT_WALL;
    }

    travel_floor = data.as_bool("stats", "travel.floor"); //Default is false so ok
    if (!travel_floor)
        travel_conditions |= NOT_FLOOR;

    vision_tint = data.as_Color("stats", "sight.vision_tint");
}
