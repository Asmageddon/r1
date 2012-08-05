#include "AttackAction.hpp"

#include "Action.hpp"

#include "../Unit.hpp"

bool AttackAction::Perform() {
    //TODO: unit->Attack(target) instead; Events; Stuff
    unsigned int damage = unit->GetStat(STAT_STRENGTH);
    damage += (1.0 * rand() / RAND_MAX - 0.5) * damage;
    target->Hurt("", damage);
    return true;
}

void AttackAction::OnAttached() {
    //TODO: Attacks, multiple this by 1.0 / attack speed
    SetTimeLeft(unit->GetMovementSpeed());
}

AttackAction::AttackAction(Unit* target) : Action(), target(target) { }
