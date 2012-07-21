#include "SightField.hpp"

#include "Field.hpp"

#include <SFML/Graphics.hpp>

SightField::SightField(): Field() {
    SetFalloff(FALLOFF_FLAT);
}

void SightField::SetLightThreshold(const float& threshold) {
    light_threshold = threshold;
    if (current_level != NULL)
        Recalculate();
}
float SightField::GetLightThreshold() const {
    return light_threshold;
}

bool SightField::GetVisibilityAt(const sf::Vector2i& pos) const {
    if (!InBounds(pos)) {
        return false;
    }
    else if (pos == origin) {
        return true;
    }
    else {
        bool visible = GetIntensityAt(pos) >= 0.1;
        if (visible) {
            sf::Color light = current_level->GetLightColorAt(pos);

            float i = std::max(light.r, std::max(light.g, light.b)) / 255.0;

            return (i > light_threshold);
        }
        else return false;
    }
}
