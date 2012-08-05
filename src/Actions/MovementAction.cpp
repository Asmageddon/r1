#include "MovementAction.hpp"

#include "Action.hpp"

#include "../Level.hpp"
#include "../Tile.hpp"
#include "../Unit.hpp"
#include "../Resources/UnitType.hpp"

MovementAction::MovementAction(const sf::Vector2i& direction) : Action() {
    vector = direction;
}

bool MovementAction::Perform() {
    Level *level = unit->GetCurrentLevel();
    const Tile& t = level->GetTile(unit->GetPosition() + vector);
    if ( t.SatisfyAll( unit->type->travel_conditions ) ) {
        unit->Move(vector);
    }
    return true;
}

bool MovementAction::PreCondition() {
    Level *level = unit->GetCurrentLevel();
    const Tile& t = level->GetTile(unit->GetPosition() + vector);
    //TODO: Also check if any potential unit would agree to a swap
    if ( t.SatisfyAll( unit->type->travel_conditions )) {
        return false;
    }
    return true;
}

void MovementAction::OnAttached() {
    SetTimeLeft(unit->GetMovementSpeed());
}
