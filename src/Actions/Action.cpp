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

    if (PreCondition()) {
        Cancel();
    }
}

void Action::Cancel() {
    finished = true;
}

void Action::SetTimeLeft(unsigned int new_time) {
    if (time_left == ntime) original_time = new_time;

    time_left = new_time;
}

bool Action::PreCondition() {
    return false;
}

void Action::OnAttached() {
    //Do nothing
}

void Action::OnTick() {
    //Do nothing
}

int Action::GetTimeLeft() const {
    return time_left;
}

bool Action::Tick() {
    if (time_left == ntime) return false;

    time_left -= 1;
    OnTick();
    if (time_left <= 0) {
        finished |= Perform();
        return finished;
    }
    return false;
}

bool Action::IsDone() const {
    return finished;
}
