#include "debug.h"
#include "engine-math.h"
#include "renderer.h"
#include "engine.h"

void Engine::ProcessInput()
{
    float speed = 1;
    float sensitivity = 3;

    if (lock) {
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            player.isCrouching = true;
        else
            player.isCrouching = false;

        sf::Vector2i mouseCoord = sf::Mouse::getPosition(window);
        sf::Vector2i windowCenter = sf::Vector2i(window.getSize() / 2u);
        player.Rotate((windowCenter.x - mouseCoord.x) * DEG_TO_RAD * dt.asSeconds() * sensitivity);
        sf::Mouse::setPosition(windowCenter, window);
    }
}

void Engine::ProcessEvent(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        SetCursorLock(!lock);
    }
    if (event.type == sf::Event::LostFocus) {
        SetCursorLock(false);
    }
    if (event.type == sf::Event::GainedFocus) {
        //SetCursorLock(lock);
    }
    if (event.type == sf::Event::Resized)
    {
        // update the view to the new size of the window
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
    }
}

void Engine::SetCursorLock(bool lock)
{
    this->lock = lock;
    window.setMouseCursorVisible(!lock);
    window.setMouseCursorGrabbed(lock);
}

Engine::Engine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "25d-engine", sf::Style::Default, sf::ContextSettings(0,0,2))
{
}

void Engine::Run()
{
    player.MoveTo(sf::Vector2f(8.5, 2.5));
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
            ProcessEvent(event);

        ProcessInput();

        window.clear();
        mainRenderer.Render(window, player);
        window.display();
        if (fpsClock.getElapsedTime().asSeconds() > 0.5) {
            Debug::SetMetric("FPS", std::to_string(1 / dt.asSeconds()));
            fpsClock.restart();
        }
    }
}
