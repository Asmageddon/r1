#ifndef SIMPLEDUNGEONGENERATOR_HPP_
#define SIMPLEDUNGEONGENERATOR_HPP_

#include <vector>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "Generator.hpp"

#include "../AString.hpp"

#include "../Tile.hpp"
#include "../Level.hpp"
#include "../Resources/MapType.hpp"

class SimpleDungeonGenerator: public Generator {
    private:
        Tile room_wall_tile;
        Tile corridor_wall_tile;

        Tile floor_tile;
        Tile fill_tile;

        int  wall_width;
        sf::Vector2i room_size_min; // min x, min y
        sf::Vector2i room_size_max; // max x, max y

        int room_count_min;
        int room_count_max;
    public:
        SimpleDungeonGenerator(const MapType& type, const Data& data, const std::string& category);
        virtual void GenerateLevel(Level& level);
};

#endif
