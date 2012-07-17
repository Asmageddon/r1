#include "TileSprite.hpp"

TileSprite::TileSprite() : Sprite() { }

TileSprite::TileSprite(const Texture& tex, const Vector2i& tile_size, const int& sprite) : Sprite(tex) {
    m_tile_size = tile_size;
    setTexture(tex);
    setSprite(sprite);
}

const Vector2i& TileSprite::getTileSize() {
    return m_tile_size;
}

void TileSprite::setSprite(const unsigned int& pos) {
    IntRect rect;
    Vector2u size = this->getTexture()->getSize();

    if (size.x == 0) return;

    size.x /= m_tile_size.x;
    size.y /= m_tile_size.y;

    unsigned int x = pos % size.x;
    unsigned int y = pos / size.x;

    if (pos >= size.x * size.y) {
        x = y = 0;
    }

    rect = IntRect(
        x * m_tile_size.x,
        y * m_tile_size.y,
        m_tile_size.x,
        m_tile_size.y
    );
    setTextureRect(rect);
}