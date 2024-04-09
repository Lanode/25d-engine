#include <cmath>
#include <array>
#include <SFML/Graphics.hpp>
#include "engine-math.h"

const int window_width = 200;
const int window_height = 200;

struct PlayerInfo {
    float x;
    float y;
    float camera_angle;
};

const std::array<std::array<short, 10>, 10> level = {{
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
}};

float Trace(float angle, float traveled_dist)
{
    float x, y;
    float xs, ys;
    float xn, yn;

    // horizontal tracing

    x = entry_coord;
    y = 1;
    ys = y;
    while (level[angle > 0 && angle <= 180 ? y-1 : y][(int)x] != 1) {
        xs = ys/std::tan(angle);
        x += xs;
    }
}

void Render(PlayerInfo player)
{

}

int main()
{
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "25d-engine");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    PlayerInfo player;
    player.x = 4;
    player.y = 4;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
