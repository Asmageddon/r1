#include "MovementAction.hpp"

#include "Level.hpp"
#include "Tile.hpp"
#include "Unit.hpp"
#include "UnitType.hpp"

MovementAction::MovementAction(unsigned int time, const sf::Vector2i& direction) {
    time_left = time;
    vector = direction;
}
bool MovementAction::Perform() {
    Level *level = unit->GetCurrentLevel();
    const Tile& t = level->GetTile(unit->GetPosition() + vector);
    if ( t.SatisfyAll( unit->type->travel_conditions | NO_UNIT ) ) {
        unit->Move(vector);
    }
    return true;
}
