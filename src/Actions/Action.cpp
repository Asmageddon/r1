#include "Action.hpp"

#include <SFML/Graphics.hpp>

#include "../Unit.hpp"
#include "../Resources/UnitType.hpp"
#include "../Level.hpp"
#include "../Tile.hpp"

Action::Action() : time_left(0), original_time(0), unit(NULL), finished(false) {}
Action::Action(unsigned int time) : time_left(time), original_time(time),unit(NULL), finished(false) {}
Action::~Action() {}

void Action::AttachToUnit(Unit *unit) {
    this->unit = unit;
    //This should only happen right before this action starts being executed
    if (!PreCondition()) {
        Cancel();
    }
}

void Action::Cancel() {
    unit->SetNextAction(NULL, true);
}

bool Action::Tick() {
    time_left -= 1;
    if (time_left <= 0) {
        finished = Perform();
        return finished;
    }
    return false;
}

bool Action::IsDone() {
    return finished;
}
