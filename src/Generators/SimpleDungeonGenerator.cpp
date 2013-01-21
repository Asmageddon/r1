#include "SimpleDungeonGenerator.hpp"

#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "Generator.hpp"

#include "../AString.hpp"

#include "../Tile.hpp"
#include "../Level.hpp"
#include "../Resources/MapType.hpp"

#include "../utils.hpp"

SimpleDungeonGenerator::SimpleDungeonGenerator(const MapType& type, const Data& data, const std::string& category)
:Generator(type, data, category) {
    room_wall_tile = corridor_wall_tile = const_access(type.generator_tiles, data[category]["wall_tile"] );

    room_wall_tile = const_access(type.generator_tiles, data[category]["wall_tile.room"] );
    corridor_wall_tile = const_access(type.generator_tiles, data[category]["wall_tile.corridor"] );
    floor_tile= const_access(type.generator_tiles, data[category]["floor_tile"]);
    fill_tile = const_access(type.generator_tiles, data[category]["fill_tile"] );

    //Wall width
    if (data[category].HasField("wall_width"))
        wall_width = (data[category]["wall_width"].as_int());
    else
        wall_width = 1;

    //Room size min
    if (data[category].HasField("room_size.min"))
        room_size_min = make_vector2i(data[category]["room_size.min"]);
    else
        room_size_min = sf::Vector2i(2, 2);

    //Room size max
    if (data[category].HasField("room_size.max"))
        room_size_max = make_vector2i(data[category]["room_size.max"]);
    else
        room_size_max = sf::Vector2i(2, 2);

    //Room count
    if (data[category].HasField("room_count"))
        room_count_min = room_count_max = data[category]["room_count"].as_int();
    else
        room_count_min = room_count_max = 1;

    //Room count min and max
    if (data[category].HasField("room_count.min"))
        room_count_min = data[category]["room_count.min"].as_int();
    if (data[category].HasField("room_count.max"))
        room_count_max = data[category]["room_count.max"].as_int();
}

void SimpleDungeonGenerator::GenerateLevel(Level& level) {
    const sf::Vector2u& size = level.GetSize();

    const Tile& void_tile = const_access(level.GetMapType()->generator_tiles, "void");

    int rooms = rand() % (room_count_max - room_count_min) + room_count_min;

    for(int i = 0; i < rooms; i++) {
        int size_x = rand() % (room_size_max.x - room_size_min.x) + room_size_min.x;
        int size_y = rand() % (room_size_max.y - room_size_min.y) + room_size_min.y;
    }
}
