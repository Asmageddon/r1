#ifndef WANDERERAI_HPP_
#define WANDERERAI_HPP_

class Unit;

#include "BaseAI.hpp"

//TODO: Create an AI capable of emitting actions, intercepting events, etc. etc.

class WandererAI : public BaseAI {
    protected:
        virtual void OnTick();
};

#endif
