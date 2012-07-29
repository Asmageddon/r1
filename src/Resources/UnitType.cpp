#include "UnitType.hpp"

#include "../AString.hpp"
#include <vector>

#include "Resource.hpp"
#include "Displayable.hpp"
#include "Glowable.hpp"

#include "../TileSprite.hpp"

#include "ResourceManager.hpp"
#include "../Tile.hpp" //We need NOT_FLOOR and NOT_WALL. TODO: Investigate if this can't be done another way

#include "../utils.hpp"

UnitType::UnitType(ResourceManager *resman, Data data) : Resource(data), Displayable(data), Glowable(data) {
    this->resman = resman;

    categories = data[""]["categories"].as_string_vector();

    material = data["body"]["material"];

    sprite = resman->GetSprite(tileset, image);

    sight_radius = data["stats"]["sight.radius"].as_int();

    if (data["stats"].HasField("sight.threshold")) {
        sight_threshold = data["stats"]["sight.threshold"].as_float();
    }
    else {
        sight_threshold = 0.33;
    }

    travel_conditions = 0;

    if (data["stats"].HasField("travel.floor")) {
        travel_floor = data["stats"]["travel.floor"].as_bool();
        if (!travel_floor)
            travel_conditions |= NOT_FLOOR;
    }
    else {
        travel_floor = true;
    }

    travel_walls = data["stats"]["travel.walls"].as_bool(); //Default is false so ok
    if (!travel_walls)
        travel_conditions |= NOT_WALL;

    vision_tint = make_color(data["stats"]["sight.vision_tint"]);

    ai = data["behavior"]["ai"];
    ai_swap_policy = data["behavior"]["ai.swap_policy"];

    if (data["stats"].HasField("travel.speed"))
        movement_speed = data["stats"]["travel.speed"].as_int();
    else
        movement_speed = 10;

    if (data["stats"].HasField("perception")) {
        std::vector<int> v = data["stats"]["perception"].as_int_vector();
        perception_min = v[0];
        perception_max = v[1];
    }
    else {
        perception_min = 5;
        perception_min = 10;
    }
}
