#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <SFML/Graphics.hpp>

class Unit;

class Action {
    public:
        static const unsigned int ntime = -1;
    protected:
        unsigned int time_left;
        unsigned int original_time;
        Unit *unit;
        bool finished;
    private:
        //Everything that returns bool returns true for done/cancelled and false otherwise
        virtual bool Perform() = 0;
        virtual bool PreCondition();
        virtual void OnAttached();
        virtual void OnTick();
    public:
        Action();
        Action(unsigned int time);
        virtual ~Action();
        void AttachToUnit(Unit *unit);

        //These might not make much sense when used outside Action and subclasses, but in no casy is any harm done exposing them ^^
        void Cancel();
        void SetTimeLeft(unsigned int new_time);
        int GetTimeLeft() const;

        bool Tick();
        bool IsDone() const;
};

#endif
