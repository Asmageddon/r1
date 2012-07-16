#include "Unit.hpp"

#include <set>

#include "Level.hpp"
#include "LightField.hpp"

#include "ResourceManager.hpp"

Unit::Unit(Level *location, const std::string& type_id) {
    //TODO: Stats
    //TODO: Other useful properties
    type = &location->resman->GetUnitType(type_id);
    material = &location->resman->GetMaterial(type->material);

    integrity = 0;
    temperature = 0;

    hp = 1;
    this->pos = sf::Vector2i(0,0);

    this->location = location;

    LightField *field = NULL;
    if (material->glow_radius > 0) {
        field = new LightField();

        field->SetFalloff(FALLOFF_LINEAR_SMOOTH);

        field->SetRadius(material->glow_radius);
        field->SetColor(material->glow_color);

        AttachLight(field);
    }
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

    //Recalculate light fields
    std::set<LightField*>::iterator it = lights.begin();
    for (; it != lights.end(); it++) {
        (*it)->Calculate(location, pos);
    }

    //Do the same for the other unit
    if (tmp != NULL) {
        it = tmp->lights.begin();
        for (; it != tmp->lights.end(); it++) {
            (*it)->Calculate(location, pos);
        }
    }
}


void Unit::SetPosition(const sf::Vector2i& new_pos) {
    Move(new_pos - pos);
}
const sf::Vector2i& Unit::GetPosition() const {
    return pos;
}

//void Unit::SetLocation(const std::string& loc) {

//}
//const std::string& Unit::GetLocation() const;

void Unit::AttachLight(LightField *light) {
    light->Calculate(location, pos);
    this->lights.insert(light);
    location->lights.insert(light);
}
void Unit::DetachLight(LightField *light) {
    this->lights.erase(light);
    location->lights.erase(light);
}