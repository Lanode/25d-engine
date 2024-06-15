#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../math/math.h"
#include "../scene.h"

class Entity
{
protected:
    sf::Vector3f coord; ///< Coordinates of player in world
    sf::Vector3f rotation; ///< Rotation of player in euler radians

	std::shared_ptr<Scene> scene;

    /**
     * @brief ProcessCollide
     * @param newCoord
     * @return Returns avability of moving
     */
    bool ProcessCollide(sf::Vector3f newCoord);
public:
	Entity(std::shared_ptr<Scene> scene);

    void MoveTo(sf::Vector3f coord);
    void MoveRelative(sf::Vector3f vec);
    sf::Vector3f GetCoord();
    void RotateTo(sf::Vector3f rotation);
    void RotateRelative(sf::Vector3f rotation);
    sf::Vector3f GetRotation();
};
