#include "Action.hpp"

#include "Unit.hpp"
#include "UnitType.hpp"
#include "Level.hpp"
#include "Tile.hpp"

#include <SFML/Graphics.hpp>

Action::Action() : time_left(0), unit(NULL), finished(false) {}
Action::Action(unsigned int time) : time_left(time), unit(unit), finished(false) {}
Action::~Action() {}

void Action::AttachToUnit(Unit *unit) {
    this->unit = unit;
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
