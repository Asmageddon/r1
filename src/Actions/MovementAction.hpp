#ifndef MOVEMENTACTION_HPP_
#define MOVEMENTACTION_HPP_

#include <SFML/Graphics.hpp>

#include "Action.hpp"

class MovementAction : public Action {
    protected:
        sf::Vector2i vector;
    private:
        virtual bool Perform();
        virtual bool PreCondition();
        virtual void OnAttached();

    public:
        MovementAction(const sf::Vector2i& direction);
};

#endif
