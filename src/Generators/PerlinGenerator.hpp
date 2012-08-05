#ifndef PERLINGENERATOR_HPP_
#define PERLINGENERATOR_HPP_

#include <vector>

#include <noise/noise.h>

#include "Generator.hpp"

#include "../AString.hpp"

#include "../Tile.hpp"
#include "../Level.hpp"
#include "../Resources/MapType.hpp"

class PerlinGenerator: public Generator {
    private:
        noise::module::Perlin perlin;

        std::vector<float> levels;
        std::vector<Tile> tiles;
    public:
        PerlinGenerator(const MapType& type, const Data& data, const std::string& category);
        virtual void GenerateLevel(Level& level);
};

#endif
