#ifndef DISPLAYABLE_HPP_
#define DISPLAYABLE_HPP_

#include <vector>
#include "../AString.hpp"

#include <SFML/Graphics.hpp>

class Data;

#include "../TileSprite.hpp"

class Displayable {
    public:
        AString tileset;
        int image;
        std::vector<int> variants;

        bool blocks_sight;

        TileSprite sprite;
        //TODO: Make Displayable constructor take a ResourceManager as an argument and store the ref

        AString border;
    public:
        Displayable() {};
        Displayable(Data data);
};

#endif
