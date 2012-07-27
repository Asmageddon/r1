#include "PlayerAI.hpp"

#include "BaseAI.hpp"

PlayerAI::PlayerAI() : BaseAI() {
    SetSwapPolicy(SWAP_WHEN_FORCED);
}
