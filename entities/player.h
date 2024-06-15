#pragma once
#include "entity.h"

class Player: public Entity {
private:

public:
    using Entity::Entity;

    void RotateTo(sf::Vector3f rotation);
    void RotateRelative(sf::Vector3f rotation);
};
