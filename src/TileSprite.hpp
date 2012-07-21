#ifndef TILESPRITE_HPP_
#define TILESPRITE_HPP_

#include <SFML/Graphics.hpp>

enum BORDER_POS {
    BORDER_BOTTOM = 0,
    BORDER_TOP = 1,
    BORDER_LEFT = 2,
    BORDER_RIGHT = 3,

    BORDER_CORNER_BOTTOM_LEFT = 4,
    BORDER_CORNER_TOP_LEFT = 5,
    BORDER_CORNER_TOP_RIGHT = 6,
    BORDER_CORNER_BOTTOM_RIGHT = 7,

    BORDER_ALL_BUT_BOTTOM = 8,
    BORDER_ALL_BUT_TOP = 9,
    BORDER_ALL_BUT_RIGHT = 10,
    BORDER_ALL_BUT_LEFT = 11,

    BORDER_BOTTOM_LEFT = 12,
    BORDER_TOP_LEFT = 13,
    BORDER_TOP_RIGHT = 14,
    BORDER_BOTTOM_RIGHT = 15,

    BORDER_SIDES_HORIZ = 16,
    BORDER_SIDES_VERT = 17,
    BORDER_ALL = 18,
    BORDER_EMPTY = 19
};

class TileSprite : public sf::Sprite {
    private:
        sf::Vector2i m_tile_size;
    public:
        TileSprite();
        TileSprite(const sf::Texture& tex, const sf::Vector2i& tile_size, const int& sprite = 0);
        const sf::Vector2i& getTileSize();
        void setSprite(const unsigned int& pos);
};

#endif
