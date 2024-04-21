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

    void ProcessInput();
public:
    Engine();
	void Run();
};