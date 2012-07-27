#include "Action.hpp"

#include <SFML/Graphics.hpp>

#include "../Unit.hpp"
#include "../Resources/UnitType.hpp"
#include "../Level.hpp"
#include "../Tile.hpp"

Action::Action() : time_left(ntime), original_time(ntime), unit(NULL), finished(false) {}
Action::Action(unsigned int time) : time_left(time), original_time(time),unit(NULL), finished(false) {}
Action::~Action() {}

void Action::AttachToUnit(Unit *unit) {
    this->unit = unit;
    //This should only happen right before this action starts being executed
    OnAttached();

    if (!PreCondition()) {
        Cancel();
    }
}

void Action::Cancel() {
    unit->SetNextAction(NULL, true);
}

void Action::SetTimeLeft(unsigned int new_time) {
    if (time_left == ntime) original_time = new_time;

    time_left = new_time;
}

bool Action::PreCondition() {
    return true;
}

void Action::OnAttached() {
    //Do nothing
}


bool Action::Tick() {
    if (time_left == ntime) return false;

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
