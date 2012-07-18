#include "Unit.hpp"

#include <set>

#include "Level.hpp"
#include "SightField.hpp"
#include "LightField.hpp"

#include "World.hpp"

#include "ResourceManager.hpp"

Unit::Unit(World *world, const std::string& type_id) {
    this->world = world;

    type = &(world->GetResman()->GetUnitType(type_id));
    material = &(world->GetResman()->GetMaterial(type->material));

    pos = sf::Vector2i(0,0);

    location = NULL;

    fov = new SightField();
    fov->SetLightThreshold(type->sight_threshold);
    fov->SetRadius(type->sight_radius);

    //Add material glow
    if (material->glow_radius > 0) {
        LightField *field = new LightField();

        field->SetFalloff(FALLOFF_LINEAR_SMOOTH);

        field->SetRadius(material->glow_radius);
        field->SetColor(material->glow_color);

        AttachLight(field);
    }

    //Add unit type glow
    if (type->glow_radius > 0) {
        LightField *field = new LightField();

        field->SetFalloff(FALLOFF_LINEAR_SMOOTH);

        field->SetRadius(type->glow_radius);
        field->SetColor(type->glow_color);

        AttachLight(field);
    }
}

Unit::~Unit() {
    //Note to self: Delete any items that glow before this
    std::set<LightField*>::iterator it = lights.begin();

    for (; it != lights.end(); it++) {
        if (this->location != NULL) {
            this->location->DetachLight(*it);
        }
        delete (*it);
    }

    delete fov;
}

void Unit::Move(const sf::Vector2i& vec) {
    //TODO: Make this cleaner
    //TODO: This should DESTROY unit at the destination, add Unit::Swap for swapping
    //TODO: Move this code to SetPosition
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
    fov->Calculate(location, pos);

    //Do the same for the other unit
    if (tmp != NULL) {
        it = tmp->lights.begin();
        for (; it != tmp->lights.end(); it++) {
            (*it)->Calculate(location, tmp->pos);
        }

        tmp->fov->Calculate(location, tmp->pos);
    }
}


void Unit::SetPosition(const sf::Vector2i& new_pos) {
    Move(new_pos - pos);
}

void Unit::SetPosition(const std::string& landmark) {
    if (location == NULL) return;
    SetPosition(location->GetLandmark(landmark));
}

const sf::Vector2i& Unit::GetPosition() const {
    return pos;
}

void Unit::SetLocation(const std::string& loc_id, const sf::Vector2i pos) {
    Level *new_location = world->GetLevel(loc_id);

    if (location != NULL) {
        int width = location->GetSize().x;
        long i = this->pos.x + width * this->pos.y;
        location->data[i].unit = NULL;
    }

    this->location = new_location;
    int width = location->GetSize().x;
    location->data[this->pos.x + width * this->pos.y].unit = this; //INVESTIGATE: If this is necessary or not
    SetPosition(pos);

    std::set<LightField*>::iterator it = lights.begin();

    for (; it != lights.end(); it++) {
        if (this->location != NULL) {
            this->location->DetachLight(*it);
        }
        new_location->AttachLight(*it);
    }

    fov->Calculate(new_location, pos);
}

void Unit::SetLocation(const std::string& loc_id, const std::string landmark) {
    Vector2i pos = world->GetLevel(loc_id)->GetLandmark(landmark);
    SetLocation(loc_id, pos);
}

std::string Unit::GetLocation() const {
    if (location != NULL)
        return location->id;
    else
        return "";
}

Level* Unit::GetCurrentLevel() {
    return location;
}

void Unit::AttachLight(LightField *light) {
    this->lights.insert(light);
    if (this->location == NULL) return;

    light->Calculate(location, pos);
    location->lights.insert(light);
}

void Unit::DetachLight(LightField *light) {
    this->lights.erase(light);

    if (this->location == NULL) return;
    location->lights.erase(light);
}

bool Unit::CanSee(const sf::Vector2i& pos) {
    if (location == NULL) return false;

    return fov->GetVisibilityAt(pos);
}

//TODO: Expose entirety of fov, perhaps as a const ref
float Unit::GetLightThreshold() const {
    return fov->GetLightThreshold();
}
