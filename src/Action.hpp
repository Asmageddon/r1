#ifndef ACTION_HPP_
#define ACTION_HPP_

class Unit;

#include <SFML/Graphics.hpp>

class Action {
    protected:
        unsigned int time_left;
        Unit *unit;
        bool finished;
    private:
        virtual bool Perform() = 0; //Returns false if it's not done - for example if it wants to be ran once more(for example run action)
    public:
        Action();
        Action(unsigned int time);
        virtual ~Action();
        void AttachToUnit(Unit *unit);

        bool Tick();
        bool IsDone();
};

#endif
