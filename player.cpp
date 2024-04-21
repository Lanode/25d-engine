#include "engine-math.h"
#include "level-loader.h"
#include "player.h"

void Player::Rotate(float angle)
{
    this->cameraAngle = CycleAngle(this->cameraAngle + angle);
}

float Player::GetRotation()
{
    return this->cameraAngle;
}

bool Player::ProcessCollide(sf::Vector2f newCoord)
{
    if (level[(int)newCoord.y][(int)newCoord.x] != 1)
        return true;
    else
        return false;
}

sf::Vector2f Player::GetCoord()
{
    return this->coord;
}

void Player::MoveTo(sf::Vector2f coord)
{
    this->coord = coord;
}

void Player::MoveRelative(sf::Vector2f vec)
{
    sf::Transform playerTransform;
    playerTransform.translate(this->coord);
    playerTransform.rotate(-this->cameraAngle * RAD_TO_DEG);

    /*sf::Vector2f forward = sf::Vector2f(std::cos(cameraAngle), std::sinf(cameraAngle));
    sf::Vector2f right = forward*/
    sf::Vector2f newCoord = playerTransform.transformPoint(vec);
    if (ProcessCollide(newCoord))
        this->coord = newCoord;
}
