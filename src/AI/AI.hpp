#ifndef AIABC_HPP_
#define AIABC_HPP_

class Unit;

//TODO: Create an AI capable of emitting actions, intercepting events, etc. etc.

enum SWAP_POLICY {
    SWAP_NEVER,
    SWAP_ALWAYS,
    SWAP_WHEN_FORCED, //When alternative to not swapping is being attacked
    SWAP_TEAM, //Only swap with team members
    SWAP_FRIENDS //Swap with any friendly unit
};

class AI {
    protected:
        Unit *unit;
        SWAP_POLICY swap_policy;
    protected:
        virtual void OnTick();
        virtual bool OnRequestSwap(Unit *unit, bool forced);
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

        void SetSwapPolicy(SWAP_POLICY new_policy);
        SWAP_POLICY GetSwapPolicy() const;
};

#endif
