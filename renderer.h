#pragma once
#include <SFML/Graphics.hpp>
#include "engine-math.h"
#include "player.h"

enum CastType { ctHorizontal, ctVertical };

struct RayResult {
    float angle;
    CastType castType;
    bool hasTouch = false;
    float distance = FLT_MAX;
    sf::Vector2f touchCoord;
    sf::Vector2f cellCoord;
};

class Renderer
{
private:
    bool RayCollided(CastType castType, sf::Vector2f touchCoord, float angle, RayResult& rayResult);
    bool IsRayOutOfBounds(sf::Vector2f touchCoord);
    RayResult CastRay(CastType castType, sf::Vector2f playerCoord, float angle);
    RayResult RunRayCast(sf::Vector2f playerCoord, float angle);
    void RenderDebug(sf::RenderWindow& window, Player& player, std::vector<RayResult>& rays);
    void RenderWorld(sf::RenderWindow& window, Player& player, std::vector<RayResult>& rays);
public:
    void Render(sf::RenderWindow& window, Player& player);
};