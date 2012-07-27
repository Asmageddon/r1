#include "AI.hpp"

#include <cassert>

#include "../Unit.hpp"

AI::AI() : unit(NULL) {}
AI::~AI() {}

void AI::AttachToUnit(Unit *unit) {
    //This shouldn't happen, but better safe than sorry
    assert(this->unit == NULL);
    this->unit = unit;
}

bool AI::RequestSwap(Unit *unit, bool forced) {
    return OnRequestSwap(unit, forced);
}

void AI::Tick() {
    this->OnTick();
}
