#include "Unit.hpp"

#include <set>
#include <queue>

#include "AString.hpp"

#include "Level.hpp"
#include "SightField.hpp"
#include "LightField.hpp"

#include "World.hpp"

#include "Resources.hpp"

#include "AI.hpp"
#include "Actions.hpp"

void Unit::UnitStat::operator=(const Stat& type_stat) {
    value = type_stat.value;

    potential = type_stat.potential;

    value += ((float)rand() / RAND_MAX) * 2.0f * type_stat.variation;
    value -= type_stat.variation;
}

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

    ai = NULL;

    if (type->ai == "player") AttachAI(new PlayerAI());
    else if (type->ai == "wanderer") AttachAI(new WandererAI());
    else if (type->ai == "none") AttachAI(new AI());
    else AttachAI(new AI());

    if (ai != NULL) {
        if (type->ai_swap_policy == "always") ai->SetSwapPolicy(SWAP_ALWAYS);
        else if (type->ai_swap_policy == "never") ai->SetSwapPolicy(SWAP_NEVER);
        else if (type->ai_swap_policy == "forced") ai->SetSwapPolicy(SWAP_WHEN_FORCED);
        else if (type->ai_swap_policy == "friends") ai->SetSwapPolicy(SWAP_FRIENDS);
        else if (type->ai_swap_policy == "team") ai->SetSwapPolicy(SWAP_TEAM);
    }

    strength = type->strength;
    endurance = type->endurance;
    agility = type->agility;
    dexterity = type->dexterity;

    health = GetStat(STAT_MAX_HP);
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

    if (location != NULL) {
        long i = pos.x + location->GetSize().x * pos.y;

        location->data[i].unit = NULL;
        location->units.erase(this);
    }
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
    if (actions.empty()) {
        actions.push(action);
        action->AttachToUnit(this);
    }
    else {
        if (interrupt) {
            actions.front()->Cancel();
            actions.push(action);
        }
        else {
            actions.push(action);
        }
    }
}
const Action* Unit::GetNextAction() const {
    if (actions.size() > 0)
        return actions.front();
    else
        return NULL;
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
    if (ai != NULL) ai->Tick();

    if (actions.empty()) return;

    if (!actions.front()->IsDone()) {
        actions.front()->Tick();
    }
    else {
        delete actions.front();
        actions.pop();
        if (actions.size() > 0)
            actions.front()->AttachToUnit(this);
    }
}

int Unit::GetMovementSpeed() const {
    //Damn, just can't get this right
    float v = 1.0 + (10.0 + agility.value) / (type->size * material->density);
    v = 100.0 / log(v);
    return (int)v;
}


void Unit::SetStat(UNIT_STAT stat, float value) {
    switch(stat) {
        case STAT_STRENGTH:
            strength.value = value;
            break;
        case STAT_ENDURANCE:
            endurance.value = value;
            break;
        case STAT_AGILITY:
            agility.value = value;
            break;
        case STAT_DEXTERITY:
            dexterity.value = value;
            break;
        default:
            break;
    }
}

float Unit::GetStat(UNIT_STAT stat) const {
    switch(stat) {
        case STAT_STRENGTH:
            return strength.value;
        case STAT_ENDURANCE:
            return endurance.value;
        case STAT_AGILITY:
            return agility.value;
        case STAT_DEXTERITY:
            return dexterity.value;

        case CURRENT_HP:
            return health;

        case STAT_MAX_HP:
            return floor(material->durability * log(1.0 + type->size) * endurance.value + 0.5);
        case STAT_RESILIENCE:
            return material->resilience;
        case STAT_SPEED:
            return GetMovementSpeed();
        default:
            //TODO: Algorithms for computing product stats
            return 0.0f;
    }
}

void Unit::SetStatPotential(UNIT_STAT stat, float potential) {
    switch(stat) {
        case STAT_STRENGTH:
            strength.potential = potential;
            break;
        case STAT_ENDURANCE:
            endurance.potential = potential;
            break;
        case STAT_AGILITY:
            agility.potential = potential;
            break;
        case STAT_DEXTERITY:
            dexterity.potential = potential;
            break;
        default:
            break;
    }
}

float Unit::GetStatPotential(UNIT_STAT stat) const {
    switch(stat) {
        case STAT_STRENGTH:
            return strength.potential;
        case STAT_ENDURANCE:
            return endurance.potential;
        case STAT_AGILITY:
            return agility.potential;
        case STAT_DEXTERITY:
            return dexterity.potential;
        default:
            return 0.0f;
    }
}

const float TRAIN_POTENTIAL_MULT = 0.75;

float Unit::TrainStat(UNIT_STAT stat, float amount) {
    //FIXME: Rather than (0.9 + 0.5) do (0.9 + 0.1, 1.0 + 0.4)
    int initial_value;
    float difference = 0.0;
    switch(stat) {
        case STAT_STRENGTH:
            initial_value = strength.value;
            strength.value += amount * strength.potential;
            difference = amount * strength.potential;
            if ( (int)strength.value > initial_value )
                strength.potential *= TRAIN_POTENTIAL_MULT;
            break;
        case STAT_ENDURANCE:
            initial_value = endurance.value;
            endurance.value += amount * endurance.potential;
            difference = amount * endurance.potential;
            if ( (int)endurance.value > initial_value )
                endurance.potential *= TRAIN_POTENTIAL_MULT;
            break;
        case STAT_AGILITY:
            initial_value = agility.value;
            agility.value += amount * agility.potential;
            difference = amount * agility.potential;
            if ( (int)agility.value > initial_value )
                agility.potential *= TRAIN_POTENTIAL_MULT;
            break;
        case STAT_DEXTERITY:
            initial_value = dexterity.value;
            dexterity.value += amount * dexterity.potential;
            difference = amount * dexterity.potential;
            if ( (int)dexterity.value > initial_value )
                dexterity.potential *= TRAIN_POTENTIAL_MULT;
            break;
        default:
            break;
    }

    return difference;
}

void Unit::Die() {
    //TODO: Drop items (once they're done)
    delete this;
}

bool Unit::Hurt(const std::string& damage_type, float amount) {
    //TODO: Care about damage type
    amount -= material->resilience;

    if (amount < 0) return false;
    health -= amount;

    if (health < 0.0f) {
        Die();
        return true;
    }

    return false;
}

void Unit::Heal(float amount) {
    health += amount;
    if (health > GetStat(STAT_MAX_HP))
        health = GetStat(STAT_MAX_HP);
}
