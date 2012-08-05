#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "../AString.hpp"

#include <SFML/Graphics.hpp>

#include "../Data.hpp"
#include "Resource.hpp"
#include "Glowable.hpp"

class Material : public Resource, public Glowable {
    public:
        AString type;
        sf::Color color;

        float density; //Density x Size = weight

        float durability; //Amount of hp/integrity
        float resilience; //Damage absorption

        float sharpness; //How sharp it is (for cutting weapons
        float elasticity; //How elastic it is. Affects agility

        //TODO: heat, electricity conduction, etc. etc.
    public:
        Material() { /* WARNING: This does not initialize anything */};
        Material(Data data);
};

#endif
