#include "TileSprite.hpp"

TileSprite::TileSprite() : sf::Sprite() { }

TileSprite::TileSprite(const sf::Texture& tex, const sf::Vector2i& tile_size, const int& sprite) : Sprite(tex) {
    m_tile_size = tile_size;
    setTexture(tex);
    setSprite(sprite);
}

const sf::Vector2i& TileSprite::getTileSize() {
    return m_tile_size;
}

void TileSprite::setSprite(const unsigned int& pos) {
    sf::IntRect rect;
    sf::Vector2u size = this->getTexture()->getSize();

    if (size.x == 0) return;

    size.x /= m_tile_size.x;
    size.y /= m_tile_size.y;

    unsigned int x = pos % size.x;
    unsigned int y = pos / size.x;

    if (pos >= size.x * size.y) {
        x = y = 0;
    }

    rect = sf::IntRect(
        x * m_tile_size.x,
        y * m_tile_size.y,
        m_tile_size.x,
        m_tile_size.y
    );
    setTextureRect(rect);
}
