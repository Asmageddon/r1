#include "SmartAction.hpp"

#include <SFML/Graphics.hpp>

#include "MovementAction.hpp"
#include "AttackAction.hpp"

#include "../Level.hpp"
#include "../Tile.hpp"
#include "../Unit.hpp"

#include "../AI/AI.hpp"

bool SmartAction::PreCondition() {
    return false;
}

bool SmartAction::Perform() {
    const Tile& t = unit->GetCurrentLevel()->GetTile(vector + unit->GetPosition());
    if (t.unit == NULL) {
        unit->SetNextAction(new MovementAction(vector));
    }
    else if (t.unit->type->enemy) { //MEMO: Remember to fix this once you implement faction system
        unit->SetNextAction(new AttackAction(t.unit));
    }
    else {
        if (t.unit->GetAI()->RequestSwap(unit, forced)) {
            unit->SetNextAction(new MovementAction(vector));
        }
        else if (forced) {
            unit->SetNextAction(new AttackAction(t.unit));
        }
    }
    return true;
}

void SmartAction::OnAttached() {
    SetTimeLeft(1);
}

SmartAction::SmartAction(const sf::Vector2i& direction, bool forced) :
    Action(),
    vector(direction),
    forced(forced),
    subaction(NULL)
{}
