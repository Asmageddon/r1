#include "BaseAI.hpp"

#include "AI.hpp"

#include "../Unit.hpp"

void BaseAI::OnTick() {
    //Do nothing
}

bool BaseAI::OnRequestSwap(Unit *unit, bool forced) {
    if (swap_policy == SWAP_NEVER)
        return false;
    else if (swap_policy == SWAP_ALWAYS)
        return true;
    else if (swap_policy == SWAP_WHEN_FORCED)
        return forced;
    else
        return false; //For now
}

BaseAI::BaseAI() : AI(), swap_policy(SWAP_NEVER) {}

void BaseAI::SetSwapPolicy(SWAP_POLICY new_policy) {
    swap_policy = new_policy;
}

SWAP_POLICY BaseAI::GetSwapPolicy() const {
    return swap_policy;
}
