#include "GoAction.hpp"

#include <SFML/Graphics.hpp>

#include "Action.hpp"
#include "MovementAction.hpp"

#include "../Level.hpp"
#include "../Tile.hpp"
#include "../Resources/UnitType.hpp"

GoAction::GoAction(const sf::Vector2i& direction) : MovementAction(direction) { }

bool GoAction::Perform() {
    Level *level = unit->GetCurrentLevel();
    const Tile& t = level->GetTile(unit->GetPosition() + vector);
    if ( t.SatisfyAll( unit->type->travel_conditions ) ) {
        bool success = unit->Move(vector);
        if (success)
            time_left = original_time;
        return !success;
    }
    return true;
}
