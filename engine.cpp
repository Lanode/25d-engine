#include "debug.h"
#include "math/constants.h"
#include "renderer.h"
#include "scene.h"
#include "engine.h"

void Engine::ProcessInput()
{
    float speed = 1;
    float sensitivity = 3;

    if (lock) {
        sf::Vector2i mouseCoord = sf::Mouse::getPosition(*window);
        sf::Vector2i windowCenter = sf::Vector2i(window->getSize() / 2u);
        sf::Mouse::setPosition(windowCenter, *window);
        sf::Vector3f rotation;
        rotation.x = (windowCenter.x - mouseCoord.x) * DEG_TO_RAD;
        rotation.y = (windowCenter.y - mouseCoord.y) * DEG_TO_RAD;
        player->RotateRelative(rotation * dt.asSeconds() * sensitivity);

        sf::Vector3f vec;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            vec.x = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            vec.x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            vec.y = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            vec.y = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            vec.z = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            vec.z = -1;
        player->MoveRelative(vec * dt.asSeconds() * speed);
        Debug::SetMetric("pos: ", std::format("{} {}", player->GetCoord().x, player->GetCoord().y));

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        //     player->isCrouching = true;
        // else
        //     player->isCrouching = false;
    }
}

void Engine::ProcessEvent(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
        window->close();
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
        window->setView(sf::View(visibleArea));
    }
}

void Engine::SetCursorLock(bool lock)
{
    this->lock = lock;
    window->setMouseCursorVisible(!lock);
    window->setMouseCursorGrabbed(lock);
}

Engine::Engine(int argc, char *argv[])
{
    window = std::shared_ptr<sf::RenderWindow>
        (new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "25d-engine", sf::Style::Default, sf::ContextSettings(0, 0, 2)));
    resourcesManager = std::shared_ptr<ResourcesManager>(new ResourcesManager(argv[0]));
}

void Engine::Run()
{
    Debug::WriteLn("25d-vig v.0.0.0");
    resourcesManager->LoadScene("test0");

    std::shared_ptr<Scene> s = std::shared_ptr<Scene>(new Scene);
    bool sceneLoaded = s->LoadFromResources(resourcesManager);

    player = std::shared_ptr<Player>(new Player(s));

    player->MoveTo(sf::Vector3f(2, 2, 0));
    player->RotateRelative(sf::Vector3f(180 * DEG_TO_RAD, 0, 0));

    window->setMouseCursorVisible(false);
    window->setMouseCursorGrabbed(true);

    WorldRenderer worldRenderer(window, resourcesManager, s, player);
    UIRenderer uiRenderer(window, resourcesManager);
    DebugRenderer debugRenderer(window, resourcesManager);
    debugRenderer.SetPlayer(player);
    debugRenderer.SetScene(s);
    sf::Clock deltaClock;
    sf::Clock fpsClock;
    while (window->isOpen())
    {
        dt = deltaClock.restart();

        sf::Event event;
        while (window->pollEvent(event))
            ProcessEvent(event);

        ProcessInput();

        window->clear();
        worldRenderer.Render();
        uiRenderer.Render();
        debugRenderer.Render();
        window->display();
        if (fpsClock.getElapsedTime().asSeconds() > 0.5) {
            Debug::SetMetric("FPS", std::to_string(1 / dt.asSeconds()));
            fpsClock.restart();
        }
    }
}
