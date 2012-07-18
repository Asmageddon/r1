#include "Field.hpp"

#include "Level.hpp"

#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

Field::Field() {
    radius = 1;
    width = 3;
    intensity = new float[3*3];
    center = sf::Vector2i(1,1);

    current_level = NULL;
}

void Field::SetRadius(const short& radius) {
    this->radius = radius;
    this->width = 1 + radius * 2;
    this->center = sf::Vector2i(radius, radius);
    delete[] intensity;
    intensity = new float[width * width];
}
const short& Field::GetRadius() const {
    return this->radius;
}

void Field::SetFalloff(const FALLOFF& falloff) {
    this->falloff = falloff;
    if (current_level != NULL)
        Recalculate();
}
const FALLOFF& Field::GetFalloff() const {
    return this->falloff;
}

void Field::Calculate(const Level* level, const sf::Vector2i& caster_pos) {
    /*
     * Simple raycasting
     */
    current_level = level;
    origin = caster_pos;
    for(int x = 0; x < width; x++)
    for(int y = 0; y < width; y++) {
        intensity[x + width * y] = 0.0f;
    }
    intensity[center.x + width * center.y] = 1.0f;
    int rays = M_PI * 2 * radius * 1.6;
    for(int a = 0; a < rays; a++) {
        //TODO: Precalculate these values for various ray counts and/or angles
        float ax = sin(a * M_PI * 2 / rays);
        float ay = cos(a * M_PI * 2 / rays);
        float x = center.x;
        float y = center.y;
        for(int z = 0; z < this->radius; z++) {
            x += ax;
            y += ay;
            sf::Vector2i pos = sf::Vector2i(
                (int)round(x),
                (int)round(y)
            );
            if (z == radius - 1) {
                if (sqrt((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y)) > radius) {
                    break;
                }
            }
            switch(this->falloff) {
                float dist;
                case FALLOFF_LINEAR_ROUGH:
                    intensity[pos.x + width * pos.y] = 1.0f - (z * 1.0f / this->radius);
                    break;
                case FALLOFF_LINEAR_SMOOTH:
                    dist = sqrt((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y));
                    if (dist > radius) dist = radius;
                    intensity[pos.x + width * pos.y] = 1.0f - (dist / this->radius);
                    break;
                case FALLOFF_FLAT:
                    intensity[pos.x + width * pos.y] = 1.0f;
                    break;
            }

            sf::Vector2i map_pos = pos + caster_pos - center;

            if (level->BlocksSight(map_pos))
                break;
        }
    }
}

void Field::Recalculate() {
    Calculate(current_level, origin);
}

float Field::GetIntensityAt(const sf::Vector2i& pos) const {
    sf::Vector2i _pos = pos - origin + center;
    if (_pos.x < 0) return 0.0f;
    if (_pos.x >= width) return 0.0f;
    if (_pos.y < 0) return 0.0f;
    if (_pos.y >= width) return 0.0f;

    const float& i = intensity[_pos.x + width * _pos.y];
    return i;
}

bool Field::InBounds(const sf::Vector2i& pos) const {
    sf::Vector2i _pos = pos - origin + center;
    if (_pos.x < 0) return false;
    if (_pos.x >= width) return false;
    if (_pos.y < 0) return false;
    if (_pos.y >= width) return false;

    return true;
}

const sf::Vector2i& Field::GetPosition() const {
    return origin;
}
