#ifndef WAITACTION_HPP_
#define WAITACTION_HPP_

#include "Action.hpp"

class WaitAction : public Action{
    private:
        bool long_wait;
    private:
        virtual bool Perform(); //Returns false if it's not done - for example if it wants to be ran once more(for example run action)
        virtual bool PreCondition();
    public:
        WaitAction(unsigned int time, bool long_wait);
};

#endif
