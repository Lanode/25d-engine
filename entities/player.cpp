#include "player.h"

void Player::RotateTo(sf::Vector3f rotation)
{
    Entity::RotateTo(rotation);
    //this->rotation.y = clip(this->rotation.z);
}

void Player::RotateRelative(sf::Vector3f rotation)
{
    Entity::RotateRelative(rotation);
    //this->rotation.y = clip(this->rotation.z, -);
}
