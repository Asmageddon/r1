#include "Unit.hpp"

#include <set>

#include "Level.hpp"
#include "LightField.hpp"

#include "ResourceManager.hpp"

Unit::Unit(Level *location, const UnitType *type) : type(type) {
    //FIXME: Come up with a better way to create units

    //FIXME: ResourceManager API regarding types and materials
    material = &location->resman->GetMaterial(type->material);

    integrity = 0;
    temperature = 0;

    hp = 1;
    pos = sf::Vector2i(0,0);

    this->location = location;

    //TODO: Create material light
}

void Unit::Move(const sf::Vector2i& vec) {
    if (location == NULL) return;

    sf::Vector2i new_pos = pos + vec;

    int width = location->GetSize().x;
    long new_i = new_pos.x + width * new_pos.y;
    long old_i = pos.x + width * pos.y;

    if (!location->InBounds(new_pos)) return;

    Unit* tmp = location->data[new_i].unit;

    if (tmp != NULL)
        tmp->pos = this->pos;

    this->pos = new_pos;
    location->data[new_i].unit = this;
    location->data[old_i].unit = tmp;
    //TODO: Update light fields
}

//void SetLocation(const std::string& loc) {
    //location->world
//}

//const std::string& GetLocation() const;

void Unit::AttachLight(LightField *light) {
    //TODO: Attach them to the level as well
    this->lights.insert(light);
}
void Unit::DetachLight(LightField *light) {
    this->lights.insert(light);
}