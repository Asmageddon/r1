#ifndef AI_HPP_
#define AI_HPP_

class Unit;

//TODO: Create an AI capable of emitting actions, intercepting events, etc. etc.

class AI {
    protected:
        Unit *unit;
    protected:
        virtual void OnTick() = 0;
    public:
        AI();
        AI(Unit *unit);
        virtual ~AI();
        void AttachToUnit(Unit *unit);

        //TODO:
        //OnUnitSighted
        //OnAttacked
        //OnAttacked
        //blah blah blah...

        //Under deep consideration - don't touch
        virtual bool RequestSwap(Unit *unit);
        void Tick();
};

#endif
