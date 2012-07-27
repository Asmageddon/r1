#ifndef GOACTION_HPP_
#define GOACTION_HPP_

#include <SFML/Graphics.hpp>

#include "MovementAction.hpp"

class GoAction : public MovementAction {
    public:
        GoAction(const sf::Vector2i& direction);
    private:
        virtual bool Perform();
};

#endif
