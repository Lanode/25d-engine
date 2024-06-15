#pragma once
#include <SFML/Graphics.hpp>
#include "scene.h"
#include "player.h"

enum CastType { ctHorizontal=0, ctVertical=1 };
enum TouchSide { tsNorth=0, tsSouth=1, tsWest=2, tsEast=3 };
/*
DO NOT TOUCH! THEIR ORDER'S IMPORTANT!
ct v l0 ts ts_name
0  ↓ 0  ▔  top
0  ↑ 1  ▁  bottom
1  → 0  ▏  left
1  ← 1  ▕  right
ct - castType
l0 - lookTo0
ts - touchSide
As we see this resembles binary num from 0 to 3
so we can just set touching side with simple formula:
touchSide = 2*castType+lookTo0;
*/

struct RayResult {
    float angle;
    CastType castType;
    bool hasTouch = false;
    float distance = FLT_MAX;
    sf::Vector2f touchCoord;
    sf::Vector2i cellCoord;
    TouchSide touchSide;
};

class Raycaster
{
private:
    std::shared_ptr<Scene> scene;

    bool RayCollided(CastType castType, sf::Vector2f touchCoord, bool lookTo0, RayResult& rayResult);
    bool IsRayOutOfBounds(sf::Vector2f touchCoord);
public:
    Raycaster(std::shared_ptr<Scene> scene);
    RayResult CastRay(CastType castType, sf::Vector2f playerCoord, float angle);
    RayResult RunRaycast(sf::Vector2f playerCoord, float angle);
};

class Renderer
{
protected:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<ResourcesManager> resourcesManager;

    Renderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager);
public:
    virtual void Render() = 0;
};

class WorldRenderer: public Renderer
{
private:
    Raycaster raycaster;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Player> player;

    std::vector<RayResult> CastRays();
    void RenderWalls(std::vector<RayResult>& rays);
    void RenderSkybox();
    void RenderSprites();
public:
    WorldRenderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager,
                  std::shared_ptr<Scene> scene, std::shared_ptr<Player> player);

    void Render();
};

class UIRenderer: public Renderer
{
private:

public:
    UIRenderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager)
        : Renderer(window, resourcesManager)
    {}

    void Render();
};

class DebugRenderer: public Renderer
{
private:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Player> player;

public:
    DebugRenderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager)
        : Renderer(window, resourcesManager)
    {}

    void Render();

    void SetScene(std::shared_ptr<Scene> scene);
    void SetPlayer(std::shared_ptr<Player> player);
};
