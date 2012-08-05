#include "PerlinGenerator.hpp"

#include <vector>

#include <noise/noise.h>

#include "Generator.hpp"

#include "../AString.hpp"

#include "../Tile.hpp"
#include "../Level.hpp"
#include "../Resources/MapType.hpp"

#include "../utils.hpp"

PerlinGenerator::PerlinGenerator(const MapType& type, const Data& data, const std::string& category)
:Generator(type, data, category) {
    if (data[category].HasField("octaves"))
        perlin.SetOctaveCount(data[category]["octaves"].as_int());
    else
        perlin.SetOctaveCount(1);

    if (data[category].HasField("frequency"))
        perlin.SetFrequency(data[category]["frequency"].as_float());
    else
        perlin.SetFrequency(4.0);

    levels = data[category]["levels"].as_float_vector();

    std::vector<AString> svec = data[category]["tiles"].as_string_vector();
    tiles.resize(svec.size());

    for(unsigned int i = 0; i < svec.size(); i++) {
        tiles[i] = const_access(type.generator_tiles, svec[i]);
    }
}

void PerlinGenerator::GenerateLevel(Level& level) {
    const sf::Vector2u& size = level.GetSize();

    const Tile& void_tile = const_access(level.GetMapType()->generator_tiles, "void");

    for(unsigned int x=0; x < size.x; x++)
    for(unsigned int y=0; y < size.y; y++) {
        if (IsMasked(level, x, y)) continue;

        //TODO: Different seeds for multiple generators
        double value = perlin.GetValue (0.14 * x, 0.14 * y, level.GetSeed() * 11.973);

        for(unsigned int i = 0; i < levels.size(); i++) {
            if (value <= levels[i]) {
                if (tiles[i] != void_tile)
                    level.FastSetTile(x, y, tiles[i]);
                break;
            }
        }
    }
}
