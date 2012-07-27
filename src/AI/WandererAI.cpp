#include "WandererAI.hpp"

#include "BaseAI.hpp"

#include "../Actions.hpp"
#include "../Unit.hpp"

#include <iostream>

void WandererAI::OnTick() {
    if (unit->GetNextAction() == NULL) {
        int x = rand() % 3 - 1;
        int y = rand() % 3 - 1;

        Action *a = new MovementAction(sf::Vector2i(x, y));

        unit->SetNextAction(a);
    }
}
