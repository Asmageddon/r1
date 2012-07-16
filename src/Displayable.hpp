#ifndef DISPLAYABLE_HPP_
#define DISPLAYABLE_HPP_

#include <vector>
#include <string>
using namespace std;

#include "Data.hpp"

#include "TileSprite.hpp"

class Displayable {
    public:
        string tileset;
        int image;
        vector<int> variants;

        TileSprite sprite;
        //TODO: Make Displayable constructor take a ResourceManager as an argument and store the ref

        string border;
    public:
        Displayable() {};
        Displayable(Data data);
};

#endif