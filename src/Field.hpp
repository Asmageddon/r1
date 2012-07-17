#ifndef FIELD_HPP_
#define FIELD_HPP_

#include "Level.hpp"

#include <SFML/Graphics.hpp>

enum FALLOFF {
    FALLOFF_FLAT,
    FALLOFF_LINEAR_ROUGH,
    FALLOFF_LINEAR_SMOOTH
};

class Field {
    private:
        short radius;
        short width;
        sf::Vector2i center;
        FALLOFF falloff;
    protected:
        sf::Vector2i origin;
        const Level *current_level;
        float *intensity;
    public:
        Field();

        void SetRadius(const short& radius);
        const short& GetRadius() const;

        void SetFalloff(const FALLOFF& falloff);
        const FALLOFF& GetFalloff() const;

        void Calculate(const Level* level, const sf::Vector2i& caster_pos);
        void Recalculate();

        float GetIntensityAt(const sf::Vector2i& pos) const;

        bool InBounds(const sf::Vector2i& pos) const;

        const sf::Vector2i& GetPosition() const;
};

#endif