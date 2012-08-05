#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include <vector>
#include <string>

#include "../Resources/MapType.hpp"
#include "../Data.hpp"
#include "../Level.hpp"
#include "../Tile.hpp"

class Generator {
    protected:
        std::set<Tile> mask_whitelist; //WIP: Current
        std::set<Tile> mask_blacklist;
    protected:
        bool HasMask() const;
        bool IsMasked(const Level& level, int x, int y) const;
    public:
        Generator(const MapType& type, const Data& data, const std::string& category);
        virtual void GenerateLevel(Level& level) = 0;
        //When I add infinite/large maps:
        //virtual void GenerateChunk(MapChunk& chunk);
};

#endif
