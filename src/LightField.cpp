#include "LightField.hpp"

#include "Field.hpp"

#include <SFML/Graphics.hpp>

void LightField::SetColor(const sf::Color& col) {
    this->color = col;
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