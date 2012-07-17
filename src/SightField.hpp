#ifndef SIGHTFIELD_HPP_
#define SIGHTFIELD_HPP_

#include "Field.hpp"

#include <SFML/Graphics.hpp>

//TODO: Consider night vision ([stats] sight.vision_tint)

class SightField: public Field {
    private:
        float light_threshold;
    public:
        SightField();
        void SetLightThreshold(const float& threshold);
        float GetLightThreshold() const;
        bool GetVisibilityAt(const sf::Vector2i& pos) const;
};

#endif