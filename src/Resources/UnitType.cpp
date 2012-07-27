#include "UnitType.hpp"

#include "../AString.hpp"
#include <vector>

#include "Resource.hpp"
#include "Displayable.hpp"
#include "Glowable.hpp"

#include "../TileSprite.hpp"

#include "ResourceManager.hpp"
#include "../Tile.hpp" //We need NOT_FLOOR and NOT_WALL. TODO: Investigate if this can't be done another way

UnitType::UnitType(ResourceManager *resman, Data data) : Resource(data), Displayable(data), Glowable(data) {
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

    if (data.HasField("stats", "travel.floor")) {
        travel_floor = data.as_bool("stats", "travel.floor");
        if (!travel_floor)
            travel_conditions |= NOT_FLOOR;
    }
    else {
        travel_floor = true;
    }

    travel_walls = data.as_bool("stats", "travel.walls"); //Default is false so ok
    if (!travel_walls)
        travel_conditions |= NOT_WALL;

    vision_tint = data.as_Color("stats", "sight.vision_tint");

    ai = data.as_string("behavior", "ai");
    ai_swap_policy = data.as_string("behavior", "ai.swap_policy");
}
