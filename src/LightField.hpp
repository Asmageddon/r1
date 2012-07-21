#ifndef LIGHTFIELD_HPP_
#define LIGHTFIELD_HPP_

#include "Field.hpp"

#include <SFML/Graphics.hpp>

class LightField: public Field {
    private:
        sf::Color color;
    public:
        LightField();
        void SetColor(const sf::Color& col);
        const sf::Color& GetColor() const;
        sf::Color GetColorAt(const sf::Vector2i& pos) const;

        void Calculate(Level* level, const sf::Vector2i& caster_pos);
};

#endif
