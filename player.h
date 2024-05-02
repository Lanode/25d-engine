#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Vector2f coord;
    float cameraAngle = 0;

    bool ProcessCollide(sf::Vector2f newCoord);
public:
    bool isCrouching = false;
    sf::Vector2f GetCoord();
    void MoveTo(sf::Vector2f coord);
    void MoveRelative(sf::Vector2f vec);
    void Rotate(float angle);
    float GetRotation();
};