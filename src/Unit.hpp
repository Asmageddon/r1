#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <SFML/Graphics.hpp>

class Unit {
    public:
        u_int16_t type;
        u_int16_t material;
        u_int16_t integrity;
        u_int16_t temperature;

        u_int32_t hp;

        sf::Vector2i pos;

        //std::vector<Object> contents;

    public:
        Unit(const UnitType& prototype) { }
        Unit() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;

            hp = 1;
            pos = sf::Vector2i(0,0);

        }

        //void Move(const sf::Vector2i& vec) {
            //if (world == NULL) return;

            //sf::Vector2i new_pos = pos + vec;

            //if (!world->InBounds(new_pos)) return;

            //Unit* tmp = NULL;
            //sf::Vector2i old_pos = pos;
            //if (world->data[new_pos.x + width * new_pos.y].unit != NULL) {
                //tmp = world->data[new_pos.x + width * new_pos.y].unit;
                //tmp->pos = pos;
            //}
            //pos = new_pos;
            //world->data[new_pos.x + width * new_pos.y].unit = unit;
            //world->units[unit] = new_pos;

            //world->data[old_pos.x + width * old_pos.y].unit = NULL;
            //if (tmp != NULL) {
                //world->data[old_pos.x + width * old_pos.y].unit = tmp;
                //world->units[tmp] = old_pos;
            //}
        //}
};
#endif