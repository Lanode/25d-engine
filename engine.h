#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "player.h"

class Engine
{
private:
    sf::RenderWindow window;
    Player player;
    sf::Time dt;
    bool lock = true;

    void ProcessInput();
    void ProcessEvent(sf::Event& event);
public:
    Engine();
	void Run();
};
