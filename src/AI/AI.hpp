#ifndef AIABC_HPP_
#define AIABC_HPP_

class Unit;

//TODO: Create an AI capable of emitting actions, intercepting events, etc. etc.

class AI {
    protected:
        Unit *unit;
    protected:
        virtual void OnTick() = 0;
        virtual bool OnRequestSwap(Unit *unit, bool forced) = 0;
    public:
        AI();
        virtual ~AI();
        void AttachToUnit(Unit *unit);

        //TODO:
        //OnUnitSighted
        //OnAttacked
        //OnAttacked
        //blah blah blah...

        //Under deep consideration - don't touch
        bool RequestSwap(Unit *unit, bool forced = false);
        void Tick();
};

#endif
