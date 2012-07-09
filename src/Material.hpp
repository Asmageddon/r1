#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Object.hpp"
#include "Data.hpp"

class Material : public Object {
    public:
        string type;
        Color color;
        float opacity;
        float reflectivity;
    public:
        Material();
        Material(Data data);
};

#endif