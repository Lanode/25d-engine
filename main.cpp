#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <functional>
#include <format>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
//#include "engine-math.h"

#define RAD_TO_DEG 180/M_PI
#define DEG_TO_RAD M_PI/180

const int CELL_SIZE = 1;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int RAY_NUMBER = 1;

enum CastType {ctHorizontal, ctVertical};

struct PlayerInfo {
    sf::Vector2f coord;
    float camera_angle;
};

struct RayResult {
    float angle;
    CastType castType;
    bool hasTouch = false;
    float distance = FLT_MAX;
    sf::Vector2f touchCoord;
    sf::Vector2f cellCoord;
};

const std::array<std::array<short, 10>, 10> level = {{
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
}};

// float abs(sf::Vector2 vec)
// {
//     return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
// }

bool RayCollided(CastType castType, sf::Vector2f touchCoord, float angle, RayResult& rayResult)
{
    int disc_x = touchCoord.x/CELL_SIZE;
    int disc_y = touchCoord.y/CELL_SIZE;

    try {
        // if look up then subtract 1
        if (castType == ctHorizontal && (0 < angle && angle < M_PI)) {
            if (level[disc_y - 1][disc_x] == 1) {
                rayResult.hasTouch = true;
                rayResult.cellCoord = sf::Vector2f(disc_x, disc_y - 1);
            }
            // if look left then subtract 1
        }
        else if (castType == ctVertical && (M_PI / 2 < angle && angle < 3 * M_PI / 2)) {
            if (level[disc_y][disc_x - 1] == 1) {
                rayResult.hasTouch = true;
                rayResult.cellCoord = sf::Vector2f(disc_x - 1, disc_y);
            }
        }
        // если луч в 4 области окружности
        else {
            if (level[disc_y][disc_x] == 1) {
                rayResult.hasTouch = true;
                rayResult.cellCoord = sf::Vector2f(disc_x, disc_y);
            }
        }
    }
    catch (std::exception) {
        std::cerr << "std::out_of_range in ray" << std::endl;
    }

    return rayResult.hasTouch;
}

bool IsRayOutOfBounds(sf::Vector2f touchCoord)
{
    return (0 > touchCoord.x || touchCoord.x >= 10) ||
           (0 > touchCoord.y || touchCoord.y >= 10);
}

RayResult CastRay(CastType castType, sf::Vector2f playerCoord, float angle)
{
    int ROV = 10;
    bool look_to0;
    if (castType == ctHorizontal)
        look_to0= (0 < angle && angle < M_PI);
    else
        look_to0 = (M_PI/2 < angle && angle < 3*M_PI/2);

    std::function<float (float, float)> trig_f;
    if (castType == ctHorizontal)
        trig_f = [](float a, float x) -> float {return x/std::tan(a);};
    else
        trig_f = [](float a, float x) -> float {return x*std::tan(a);};

    float a, b; // полные кординаты луча
    float as, bs; // координаты луча при ячеечных отрезках
    float an, bn; // координаты луча при вычислении первой мили

    if (castType == ctHorizontal)
        bn = playerCoord.y - (CELL_SIZE*(playerCoord.y/CELL_SIZE));
    else
        bn = playerCoord.x - (CELL_SIZE*(playerCoord.x/CELL_SIZE));
    an = trig_f(angle, bn);

    a = an;
    b = bn;

    // если идёт вверх то надо отбавлять, а если вниз то прибавлять
    bs = look_to0 ? -CELL_SIZE : CELL_SIZE;
    as = trig_f(angle, bs);

    RayResult rayResult;
    rayResult.angle = angle;
    rayResult.castType = castType;
    float distance = -1;
    for (int i=0; i<=ROV; i++) {
        sf::Vector2f touchCoord;
        if (castType == ctHorizontal)
            touchCoord = sf::Vector2f(playerCoord.x+a, playerCoord.y+b);
        else
            touchCoord = sf::Vector2f(playerCoord.x+b, playerCoord.y-a);
        if (IsRayOutOfBounds(touchCoord)) {
            break;
        }
        if (RayCollided(castType, touchCoord, angle, rayResult)) {
            rayResult.distance = std::sqrt(std::pow(a, 2) + std::pow(b, 2));
            rayResult.touchCoord = touchCoord;
            break;
        }
        a += (castType == ctHorizontal) ? -as : as;
        b += bs;
    }

    return rayResult;
}

float CycleAngle(float angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}

RayResult RayCast(sf::Vector2f playerCoord, float angle)
{
    float dist, dist_h, dist_v;
    sf::Vector2f touchCoord, touchCoord_h, touchCoord_v;

    angle = CycleAngle(angle);

    RayResult horizontalRay = CastRay(ctHorizontal, playerCoord, angle);
    RayResult verticalRay = CastRay(ctVertical, playerCoord, angle);

    RayResult rayResult;
    if (horizontalRay.hasTouch && horizontalRay.distance < verticalRay.distance)
        rayResult = horizontalRay;
    else if (verticalRay.hasTouch && verticalRay.distance < horizontalRay.distance)
        rayResult = verticalRay;
    else
        rayResult = horizontalRay;

    return rayResult;
}

void RenderDebug(sf::RenderWindow& window, PlayerInfo& player, std::vector<RayResult>& rays)
{
    const int SCALE = 20;
    try {
        for (int x=0; x<10; x++) {
            for (int y=0; y<10; y++) {
                sf::RectangleShape cell(sf::Vector2f(SCALE, SCALE));
                cell.setPosition(x*SCALE, y*SCALE);
                cell.setOutlineThickness(-1.f);
                cell.setOutlineColor(sf::Color(135, 135, 135));
                if (level[y][x] == 1)
                    cell.setFillColor(sf::Color(75, 75, 75));
                else
                    cell.setFillColor(sf::Color::White);
                window.draw(cell);
            }
        }
    }
    catch (std::exception) {
        std::cerr << "std::out_of_range in render" << std::endl;
    }

    sf::CircleShape playerShape(SCALE/10);
    playerShape.setPosition(player.coord*(float)SCALE);
    playerShape.setFillColor(sf::Color::Red);
    window.draw(playerShape);

    for (auto r : rays) {
        if (r.hasTouch) {
            sf::RectangleShape ray(sf::Vector2f(r.distance * SCALE, SCALE / 10));
            ray.setPosition(player.coord * (float)SCALE);
            ray.setRotation(-r.angle * RAD_TO_DEG);
            ray.setFillColor(sf::Color::Green);
            window.draw(ray);
        }
    }

    sf::Font font;
    if (font.loadFromFile("ModernDOS8x8.ttf")) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::format("angle {}",
            player.camera_angle * RAD_TO_DEG));
        /*text.setString(std::format("angle {}\ntouch {}\ndist {}\ncell {} {}\nvec {} {}", 
            player.camera_angle * RAD_TO_DEG,
            rayResult.hasTouch, 
            rayResult.distance,
            rayResult.cellCoord.x, rayResult.cellCoord.y,
            rayResult.touchCoord.x, rayResult.touchCoord.y));*/
        text.setCharacterSize(15);
        text.setPosition(0, -5);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(-1);
        window.draw(text);
    }
}

void RenderWorld(sf::RenderWindow& window, PlayerInfo& player, std::vector<RayResult>& rays)
{
    for (int i = 0; i < rays.size(); i++) {
        if (rays[i].hasTouch) {
            sf::RectangleShape column(sf::Vector2f(10, WINDOW_HEIGHT - (rays[i].distance * (WINDOW_HEIGHT / 10))));
            column.setOrigin(column.getSize().x / 2, column.getSize().y / 2);
            column.setPosition(WINDOW_WIDTH/2-200+i*10, WINDOW_HEIGHT / 2);
            column.setFillColor(sf::Color::Green);
            window.draw(column);
        }
    }
}

void Render(sf::RenderWindow& window, PlayerInfo& player)
{
    std::vector<RayResult> rays;
    for (float i=player.camera_angle-20*DEG_TO_RAD; i<=player.camera_angle+20*DEG_TO_RAD; i+=1*DEG_TO_RAD)
        rays.push_back(RayCast(player.coord, i));

    RenderWorld(window, player, rays);
    RenderDebug(window, player, rays);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "25d-engine");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    PlayerInfo player;
    player.coord.x = 3*CELL_SIZE;
    player.coord.y = 8*CELL_SIZE;
    player.camera_angle = 25*DEG_TO_RAD;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.camera_angle -= 10.f*DEG_TO_RAD*dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.camera_angle += 10.f*DEG_TO_RAD*dt.asSeconds();
        player.camera_angle = CycleAngle(player.camera_angle);

        window.clear();
        window.draw(shape);
        Render(window, player);
        window.display();
    }

    return 0;
}
