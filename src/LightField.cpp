#include "LightField.hpp"

#include "Field.hpp"

#include <SFML/Graphics.hpp>

LightField::LightField() : Field() {
    SetFalloff(FALLOFF_LINEAR_SMOOTH);
}

void LightField::SetColor(const sf::Color& col) {
    this->color = col;
    //We don't want to dye our sprites transparent
    if (col.a != 255) {
        color *= sf::Color(col.a, col.a, col.a);
        color.a = 255;
    }
}
const sf::Color& LightField::GetColor() const {
    return this->color;
}
sf::Color LightField::GetColorAt(const sf::Vector2i& pos) const {
    sf::Color result = this->color;
    const float& i = this->GetIntensityAt(pos);
    result.r *= i;
    result.g *= i;
    result.b *= i;
    return result;
}