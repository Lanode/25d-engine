#include "debug.h"
#include "engine-math.h"
#include "renderer.h"
#include "engine.h"

void Engine::ProcessInput()
{
    float speed = 1;
    float sensitivity = 3;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        player.MoveRelative(sf::Vector2f(1, 0) * dt.asSeconds() * speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        player.MoveRelative(sf::Vector2f(-1, 0) * dt.asSeconds() * speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        player.MoveRelative(sf::Vector2f(0, 1) * dt.asSeconds() * speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        player.MoveRelative(sf::Vector2f(0, -1) * dt.asSeconds() * speed);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.Rotate(20.f * DEG_TO_RAD * dt.asSeconds() * sensitivity);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player.Rotate(-20.f * DEG_TO_RAD * dt.asSeconds() * sensitivity);

    sf::Vector2i mouseCoord = sf::Mouse::getPosition(window);
    sf::Vector2i windowCenter = sf::Vector2i(window.getSize() / 2u);
    player.Rotate((windowCenter.x - mouseCoord.x) * DEG_TO_RAD * dt.asSeconds() * sensitivity);
    sf::Mouse::setPosition(windowCenter, window);
}

Engine::Engine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "25d-engine", sf::Style::Default, sf::ContextSettings(0,0,1))
{
}

void Engine::Run()
{
    player.MoveTo(sf::Vector2f(8.5, 1.5));
    player.Rotate(180 * DEG_TO_RAD);

    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    Renderer mainRenderer;
    sf::Clock deltaClock;
    sf::Clock fpsClock;
    while (window.isOpen())
    {
        dt = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ProcessInput();

        window.clear();
        mainRenderer.Render(window, player);
        window.display();
        if (fpsClock.getElapsedTime().asSeconds() > 1) {
            Debug::AddDebugLine(std::to_string(1 / dt.asSeconds()));
            fpsClock.restart();
        }
    }
}