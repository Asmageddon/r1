#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <SFML/Graphics.hpp>

class Unit;

class Action {
    protected:
        unsigned int time_left;
        unsigned int original_time;
        Unit *unit;
        bool finished;
    protected:
        void Cancel(); //Cancel itself
    private:
        virtual bool Perform() = 0; //Returns false if it's not done - for example if it wants to be ran once more(for example run action)
        virtual bool PreCondition() = 0;
        //TODO: Add a virtual function to be executed every tick
    public:
        Action();
        Action(unsigned int time);
        virtual ~Action();
        void AttachToUnit(Unit *unit);

        bool Tick();
        bool IsDone();
};

#endif
