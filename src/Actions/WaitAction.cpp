#include "WaitAction.hpp"

#include "Action.hpp"

bool WaitAction::Perform() {
    return !long_wait;
}
bool WaitAction::PreCondition() {
    //TODO: Check for dangers
    return true;
}
WaitAction::WaitAction(unsigned int time, bool long_wait) : Action(time), long_wait(long_wait) { }
