#ifndef DISPLAYABLE_HPP_
#define DISPLAYABLE_HPP_

#include <vector>
#include <string>
using namespace std;

#include "Data.hpp"

class Displayable {
    public:
        string tileset;
        int image;
        vector<int> variants;

        string border;
    public:
        Displayable() {};
        Displayable(Data data);
};

#endif