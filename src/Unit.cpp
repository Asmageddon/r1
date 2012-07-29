#include "Unit.hpp"

#include <set>

#include "AString.hpp"

#include "Level.hpp"
#include "SightField.hpp"
#include "LightField.hpp"

#include "World.hpp"

#include "Resources.hpp"

#include "AI.hpp"
#include "Actions.hpp"

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

    next_action = NULL;
    ai = NULL;

    if (type->ai == "player") AttachAI(new PlayerAI());
    else if (type->ai == "wanderer") AttachAI(new WandererAI());
    else if (type->ai == "none") AttachAI(new AI());

    if (ai != NULL) {
        if (type->ai_swap_policy == "always") ai->SetSwapPolicy(SWAP_ALWAYS);
        else if (type->ai_swap_policy == "never") ai->SetSwapPolicy(SWAP_NEVER);
        else if (type->ai_swap_policy == "forced") ai->SetSwapPolicy(SWAP_WHEN_FORCED);
        else if (type->ai_swap_policy == "friends") ai->SetSwapPolicy(SWAP_FRIENDS);
        else if (type->ai_swap_policy == "team") ai->SetSwapPolicy(SWAP_TEAM);
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

bool Unit::Swap(Unit *other_unit) {
    if (location == NULL) return false;
    if (other_unit == this) return false;

    if (!other_unit->ai) return false;
    else {
        if (!other_unit->GetAI()->RequestSwap(this))
            return false;
    }

    sf::Vector2i pos2 = other_unit->pos;

    //FIXME: Fix this up so I can make Level not have to friend Unit
    int width1 = location->GetSize().x;
    int width2 = other_unit->location->GetSize().x;

    long i1 = pos.x + width1 * pos.y;
    long i2 = pos2.x + width2 * pos2.y;

    other_unit->location->data[i2].unit = this;
    this->location->data[i1].unit = other_unit;

    other_unit->pos = this->pos;
    this->pos = pos2;

    //Recalculate light fields
    std::set<LightField*>::iterator it = lights.begin();
    for (; it != lights.end(); it++) {
        (*it)->Calculate(location, pos);
    }
    fov->Calculate(location, pos);

    //Do the same for the other unit
    it = other_unit->lights.begin();
    for (; it != other_unit->lights.end(); it++) {
        (*it)->Calculate(location, other_unit->pos);
    }

    other_unit->fov->Calculate(location, other_unit->pos);

    return true;
}

bool Unit::Move(const sf::Vector2i& vec) {
    //TODO: Make this cleaner
    if (location == NULL) return false;

    sf::Vector2i new_pos = pos + vec;
    if (!location->InBounds(new_pos)) return false;

    long i = new_pos.x + location->GetSize().x * new_pos.y;

    Unit* tmp = location->data[i].unit;

    //FIXME: Implement and use Level::GetUnitAt instead
    if (tmp != NULL) {
        return Swap(tmp);
    }
    else {
        SetPosition(new_pos, true);
    }

    return true;
}


void Unit::SetPosition(const sf::Vector2i& new_pos, bool ignore_terrain) {
    sf::Vector2i _new_pos;
    if (ignore_terrain)
        _new_pos = new_pos;
    else
        //TODO: Per-unittype movement restrictions and using them here and in other places
        _new_pos = location->FindTile(new_pos, IS_FLOOR | NO_UNIT | NO_OBJECT);

    long old_i = pos.x + location->GetSize().x * pos.y;

    location->data[old_i].unit = NULL;
    this->pos = _new_pos;
    long new_i = _new_pos.x + location->GetSize().x * _new_pos.y;
    location->data[new_i].unit = this;

    //Recalculate the light fields and FoV
    std::set<LightField*>::iterator it = lights.begin();
    for (; it != lights.end(); it++) {
        (*it)->Calculate(location, pos);
    }
    fov->Calculate(location, pos);
}

void Unit::SetPosition(const std::string& landmark, bool ignore_terrain) {
    if (location == NULL) return;
    SetPosition(location->GetLandmark(landmark), ignore_terrain);
}

const sf::Vector2i& Unit::GetPosition() const {
    return pos;
}

void Unit::SetLocation(const std::string& loc_id, const sf::Vector2i new_pos, bool ignore_terrain) {
    Level *new_location = world->GetLevel(loc_id);

    if (location != NULL) {
        location->DeregisterUnit(this);
        int width = location->GetSize().x;
        long i = pos.x + width * pos.y;
        location->data[i].unit = NULL;
    }

    location = new_location;
    location->RegisterUnit(this);
    int width = location->GetSize().x;
    location->data[pos.x + width * pos.y].unit = this;
    SetPosition(new_pos, ignore_terrain);

    std::set<LightField*>::iterator it = lights.begin();

    for (; it != lights.end(); it++) {
        (*it)->Calculate(location, pos);
    }

    fov->Calculate(location, pos);
}

void Unit::SetLocation(const std::string& loc_id, const std::string& landmark, bool ignore_terrain) {
    sf::Vector2i pos = world->GetLevel(loc_id)->GetLandmark(landmark);
    SetLocation(loc_id, pos, ignore_terrain);
}

AString Unit::GetLocation() const {
    if (location != NULL)
        return location->id;
    else
        return AString("");
}

Level* Unit::GetCurrentLevel() {
    return location;
}

void Unit::AttachLight(LightField *light) {
    this->lights.insert(light);
    if (this->location == NULL) return;

    light->Calculate(location, pos);
    //location->lights.insert(light);
    //location->AttachLight(light);
}

void Unit::DetachLight(LightField *light) {
    this->lights.erase(light);

    if (this->location == NULL) return;
    //location->lights.erase(light);
    location->DetachLight(light);
}

bool Unit::CanSee(const sf::Vector2i& pos) {
    if (location == NULL) return false;

    return fov->GetVisibilityAt(pos);
}

bool Unit::CanSee(const Unit* const unit) {
    if (location != unit->location) return false;

    return CanSee(unit->pos);
}

//TODO: Expose entirety of fov, perhaps as a const ref (why?)
float Unit::GetLightThreshold() const {
    return fov->GetLightThreshold();
}

void Unit::RecalculateFOV() {
    fov->Recalculate();
}

void Unit::SetNextAction(Action *action, bool interrupt) {
    if (next_action != NULL) {
        if (!interrupt) {
            return;
        }
        else {
            delete next_action;
        }
    }

    next_action = action;
    if (action != NULL)
        next_action->AttachToUnit(this);
}
const Action* Unit::GetNextAction() const {
    return next_action;
}

void Unit::AttachAI(AI *ai) {
    if (this->ai != NULL) {
        delete this->ai;
    }
    this->ai = ai;
    ai->AttachToUnit(this);
}
void Unit::DetachAI() {
    if (this->ai != NULL) {
        delete this->ai;
    }
    this->ai = NULL;
}

AI *Unit::GetAI() {
    return this->ai;
}

void Unit::Simulate() {
    if ( (next_action == NULL) & (ai == NULL) ) return;

    if (ai != NULL) ai->Tick();

    if (next_action == NULL) return;

    if (!next_action->IsDone()) {
        next_action->Tick();
    }
    else {
        delete next_action;
        next_action = NULL;
    }
}

int Unit::GetMovementSpeed() {
    return type->movement_speed;
}
