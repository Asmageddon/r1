#ifndef ATTACKACTION_HPP_
#define ATTACKACTION_HPP_

#include "Action.hpp"

class Unit;

class AttackAction : public Action {
    protected:
        Unit* target;
    private:
        virtual bool Perform();
        virtual void OnAttached();

    public:
        AttackAction(Unit* target);
};

#endif
