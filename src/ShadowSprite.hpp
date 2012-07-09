#ifndef SHADOWSPRITE_HPP_
#define SHADOWSPRITE_HPP_

#include <SFML/Graphics.hpp>
using namespace sf;

enum SHADOW_POS {
    SHADOW_BOTTOM = 0,
    SHADOW_TOP = 1,
    SHADOW_LEFT = 2,
    SHADOW_RIGHT = 3,

    SHADOW_CORNER_BOTTOM_LEFT = 4,
    SHADOW_CORNER_TOP_LEFT = 5,
    SHADOW_CORNER_TOP_RIGHT = 6,
    SHADOW_CORNER_BOTTOM_RIGHT = 7,

    SHADOW_ALL_BUT_BOTTOM = 8,
    SHADOW_ALL_BUT_TOP = 9,
    SHADOW_ALL_BUT_RIGHT = 10,
    SHADOW_ALL_BUT_LEFT = 11,

    SHADOW_BOTTOM_LEFT = 12,
    SHADOW_TOP_LEFT = 13,
    SHADOW_TOP_RIGHT = 14,
    SHADOW_BOTTOM_RIGHT = 15,

    SHADOW_SIDES_HORIZ = 16,
    SHADOW_SIDES_VERT = 17,
    SHADOW_ALL = 18,
    SHADOW_EMPTY = 19
};

class ShadowSprite: public Sprite {
    private:
        Vector2i m_tile_size;
    public:
        ShadowSprite() : Sprite() {

        }
        ShadowSprite(const Texture& tex, const Vector2i& tile_size) : Sprite(tex) {
            m_tile_size = tile_size;
            setTexture(tex);
            setSprite(SHADOW_EMPTY);
        }
        const Vector2i& getTileSize() {
            return m_tile_size;
        }
        void setSprite(SHADOW_POS pos) {
            IntRect rect;
            if (pos > 19) {
                pos = SHADOW_EMPTY;
            }
            else {
                int x = pos % 4;
                int y = pos / 4;
                rect = IntRect(
                    x * m_tile_size.x,
                    y * m_tile_size.y,
                    m_tile_size.x,
                    m_tile_size.y
                );
            }
            setTextureRect(rect);
        }
};

#endif