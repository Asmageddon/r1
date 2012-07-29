#include "AI.hpp"

#include <cassert>

#include "../Unit.hpp"

AI::AI() : unit(NULL), swap_policy(SWAP_NEVER) {}
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

void AI::OnTick() {
    //Do nothing
}

bool AI::OnRequestSwap(Unit *unit, bool forced) {
    if (swap_policy == SWAP_NEVER)
        return false;
    else if (swap_policy == SWAP_ALWAYS)
        return true;
    else if (swap_policy == SWAP_WHEN_FORCED)
        return forced;
    else
        return false; //For now
}

void AI::SetSwapPolicy(SWAP_POLICY new_policy) {
    swap_policy = new_policy;
}

SWAP_POLICY AI::GetSwapPolicy() const {
    return swap_policy;
}
