#ifndef SMARTACTION_HPP_
#define SMARTACTION_HPP_

#include <SFML/Graphics.hpp>

#include "MovementAction.hpp"
#include "AttackAction.hpp"

class SmartAction : public Action {
    protected:
        sf::Vector2i vector;
        bool forced;
        Action *subaction;
    private:
        virtual bool Perform();
        virtual bool PreCondition();
        virtual void OnAttached();
    public:
        SmartAction(const sf::Vector2i& direction, bool forced);
};

#endif
