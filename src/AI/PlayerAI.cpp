#include "PlayerAI.hpp"

#include "AI.hpp"

PlayerAI::PlayerAI() : AI() {
    SetSwapPolicy(SWAP_WHEN_FORCED);
}
