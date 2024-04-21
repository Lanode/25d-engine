#include <functional>
#include <iostream>
#include "debug.h"
#include "level-loader.h"
#include "renderer.h"

bool Renderer::RayCollided(CastType castType, sf::Vector2f touchCoord, float angle, RayResult& rayResult)
{
    int disc_x = touchCoord.x;
    int disc_y = touchCoord.y;

    if ((castType == ctHorizontal && disc_y - 1 < 0) ||
        (castType == ctVertical && disc_x - 1 < 0))
        return rayResult.hasTouch;

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
    // ���� ��� � 4 ������� ����������
    else {
        if (level[disc_y][disc_x] == 1) {
            rayResult.hasTouch = true;
            rayResult.cellCoord = sf::Vector2f(disc_x, disc_y);
        }
    }

    return rayResult.hasTouch;
}

bool Renderer::IsRayOutOfBounds(sf::Vector2f touchCoord)
{
    return (0 > touchCoord.x || touchCoord.x >= 10) ||
        (0 > touchCoord.y || touchCoord.y >= 10);
}

RayResult Renderer::CastRay(CastType castType, sf::Vector2f playerCoord, float angle)
{
    const int ROV = 10;
    float ap, bp; // ���������� ������
    float a, b; // ������ ��������� ����
    float as, bs; // ���������� ���� ��� �������� ��������
    float an, bn; // ���������� ���� ��� ���������� ������ ����
    bool look_to0; // ��� ��������� �� ����
    std::function<float(float, float)> trig_f; // ������� ���������� ������

    if (castType == ctHorizontal) {
        ap = playerCoord.x;
        bp = playerCoord.y;
        look_to0 = (0 < angle && angle < M_PI);
        trig_f = [](float a, float x) -> float {return x / std::tan(a); };
    }
    else {
        ap = playerCoord.y;
        bp = playerCoord.x;
        look_to0 = (M_PI / 2 < angle && angle < 3 * M_PI / 2);
        trig_f = [](float a, float x) -> float {return x * std::tan(a); };
    }

    if (look_to0)
        bn = bp - std::trunc(bp);
    else
        bn = (std::trunc(bp) + 1) - bp;

    bn = look_to0 ? -bn : bn;
    an = (castType == ctHorizontal) ? -trig_f(angle, bn) : trig_f(angle, bn);

    a = an;
    b = bn;

    // ���� ��� ����� �� ���� ���������, � ���� ���� �� ����������
    bs = look_to0 ? -1 : 1;
    as = trig_f(angle, bs);

    RayResult rayResult;
    rayResult.angle = angle;
    rayResult.castType = castType;
    float distance = -1;
    for (int i = 0; i <= ROV; i++) {
        sf::Vector2f touchCoord;
        if (castType == ctHorizontal)
            touchCoord = sf::Vector2f(ap + a, bp + b);
        else
            touchCoord = sf::Vector2f(bp + b, ap - a);
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

RayResult Renderer::RunRayCast(sf::Vector2f playerCoord, float angle)
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

void Renderer::RenderDebug(sf::RenderWindow& window, Player& player, std::vector<RayResult>& rays)
{
    const int SCALE = 15;
    try {
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                sf::RectangleShape cell(sf::Vector2f(SCALE, SCALE));
                cell.setPosition(x * SCALE, y * SCALE);
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

    sf::CircleShape playerShape(SCALE / 10);
    playerShape.setOrigin(playerShape.getRadius() / 2, playerShape.getRadius() / 2);
    playerShape.setPosition(player.GetCoord() * (float)SCALE);
    playerShape.setFillColor(sf::Color::Red);
    window.draw(playerShape);

    for (int i = 0; i < rays.size(); i++) {
        if (rays[i].hasTouch) {
            sf::RectangleShape ray(sf::Vector2f(rays[i].distance * SCALE, SCALE / 10));
            ray.setOrigin(ray.getSize().y / 2, 0);
            ray.setPosition(player.GetCoord() * (float)SCALE);
            ray.setRotation(-rays[i].angle * RAD_TO_DEG);
            ray.setFillColor(sf::Color::Green);
            window.draw(ray);
        }
    }

    sf::Font font;
    if (font.loadFromFile("ModernDOS8x8.ttf")) {
        sf::Text text;
        text.setFont(font);
        text.setString(Debug::GetDebugText());
        text.setCharacterSize(15);
        text.setPosition(0, -5);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(-1);
        window.draw(text);
    }
}

void Renderer::RenderWorld(sf::RenderWindow& window, Player& player, std::vector<RayResult>& rays)
{
    float column_width = (float)WINDOW_WIDTH / rays.size();
    for (int i = 0; i < rays.size(); i++) {
        if (rays[i].hasTouch) {
            float beta = std::abs(rays[i].angle - player.GetRotation());
            float correct_distance = rays[i].distance * std::cos(beta);
            float column_height = (WINDOW_HEIGHT * 0.7) / correct_distance;

            float shade_c = rays[i].castType == ctHorizontal ? 255 - 20 : 255;
            /*color_transform = shade_c - shade_c/rays[i].distance;
            if (rays[i].distance > 20)
                shade_c = 0;*/
            sf::Color shade(shade_c, shade_c, shade_c);

            sf::RectangleShape column(sf::Vector2f(column_width, column_height));
            column.setOrigin(column.getSize().x / 2, column.getSize().y / 2);
            column.setPosition(i * column_width, WINDOW_HEIGHT / 2);
            column.setFillColor(shade);
            window.draw(column);
        }
    }
}

void Renderer::Render(sf::RenderWindow& window, Player& player)
{
    const int COLUMN_COUNT = WINDOW_WIDTH;
    std::vector<RayResult> rays;
    // for vr projection
    //for (float i = player.GetRotation() + 20 * DEG_TO_RAD; i >= player.GetRotation() - 20 * DEG_TO_RAD; i -= 0.25 * DEG_TO_RAD) {
    // for plane projection
    for (int i = COLUMN_COUNT; i > 0; i--) {
        float angle = std::atan((float)(i - COLUMN_COUNT / 2) / 400);
        RayResult ray = RunRayCast(player.GetCoord(), player.GetRotation() + angle);
        rays.push_back(ray);
    }

    RenderWorld(window, player, rays);
    RenderDebug(window, player, rays);
    Debug::ClearDebugText();
}