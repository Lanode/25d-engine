#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "resources.h"
#include "entities/player.h"

class Engine
{
private:
    std::shared_ptr<sf::RenderWindow>  window;
    std::shared_ptr<Player> player;
    std::shared_ptr<ResourcesManager> resourcesManager;
    sf::Time dt;
    bool lock = true;

    void ProcessInput();
    void ProcessEvent(sf::Event& event);
    void SetCursorLock(bool lock);
public:
    Engine(int argc, char *argv[]);
    void Run();
};
