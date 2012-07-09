#ifndef TILESPRITE_HPP_
#define TILESPRITE_HPP_

#include <SFML/Graphics.hpp>
using namespace sf;

class TileSprite : public Sprite {
    private:
        Vector2i m_tile_size;
    public:
        TileSprite();
        TileSprite(const Texture& tex, const Vector2i& tile_size, const int& sprite = 0);
        const Vector2i& getTileSize();
        void setSprite(const unsigned int& pos);
};

#endif