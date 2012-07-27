#ifndef BASEAI_HPP_
#define BASEAI_HPP_

#include "AI.hpp"

class Unit;

enum SWAP_POLICY {
    SWAP_NEVER,
    SWAP_ALWAYS,
    SWAP_WHEN_FORCED, //When alternative to not swapping is being attacked
    SWAP_TEAM, //Only swap with team members
    SWAP_FRIENDS //Swap with any friendly unit
};

class BaseAI : public AI {
    private:
        SWAP_POLICY swap_policy;
    protected:
        virtual void OnTick();
        virtual bool OnRequestSwap(Unit *unit, bool forced = false);
    public:
        BaseAI();

        void SetSwapPolicy(SWAP_POLICY new_policy);
        SWAP_POLICY GetSwapPolicy() const;
};

#endif
