#include "WaitAction.hpp"

#include "Action.hpp"

#include "../Unit.hpp"
#include "../Resources/UnitType.hpp"

bool WaitAction::Perform() {
    return !long_wait;
}
bool WaitAction::PreCondition() {
    //TODO: Check for dangers
    return true;
}

void WaitAction::OnAttached() {
    SetTimeLeft(unit->GetMovementSpeed());
}

WaitAction::WaitAction(bool long_wait) : Action(), long_wait(long_wait) { }
