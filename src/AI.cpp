#include "AI.hpp"

#include "Unit.hpp"

#include <cassert>

AI::AI() : unit(NULL) {}
AI::AI(Unit *unit) : unit(unit) {}
AI::~AI() {}

void AI::AttachToUnit(Unit *unit) {
    //This shouldn't happen, but better safe than sorry
    assert(this->unit == NULL);
    this->unit = unit;
}

bool AI::RequestSwap(Unit *unit) {
    return false;
}

void AI::Tick() {
    this->OnTick();
}
