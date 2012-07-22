#ifndef MOVEMENTACTION_HPP_
#define MOVEMENTACTION_HPP_

#include "Action.hpp"

#include <SFML/Graphics.hpp>

class MovementAction : public Action {
    private:
        sf::Vector2i vector;
    private:
        virtual bool Perform();
    public:
        MovementAction(unsigned int time, const sf::Vector2i& direction);
};

#endif
