#include <functional>
#include <iostream>
#include "debug.h"
#include "map.h"
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
    // если луч в 4 области окружности
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
    float ap, bp; // координаты игрока
    float a, b; // полные кординаты луча
    float as, bs; // координаты луча при ячеечных отрезках
    float an, bn; // координаты луча при вычислении первой мили
    bool look_to0; // луч направлен на ноль
    std::function<float(float, float)> trig_f; // функция вычисления катета

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

    // если идёт вверх то надо отбавлять, а если вниз то прибавлять
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
    const int SCALE = window.getSize().x / 40;
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
        text.setString(Debug::GetMetricsAsText());
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
    sf::Texture wallTexture;
    wallTexture.loadFromFile("pic.bmp");

    // отрисовка фона
    sf::RectangleShape sky(sf::Vector2f(window.getSize().x, window.getSize().y / 2));
    sky.setFillColor(sf::Color::Cyan);
    window.draw(sky);
    sf::RectangleShape ground(sf::Vector2f(window.getSize().x, window.getSize().y / 2));
    ground.setPosition(sf::Vector2f(0, window.getSize().y/2));
    ground.setFillColor(sf::Color::Green);
    window.draw(ground);

    // вычисление и отрисовка полосок
    int viewportHeight = (window.getSize().x * 9.f) / 16.f; // 16:9 = x:y // нужно для того чтобы высота блоков не менялась от изменения высоты
    float stripe_width = (float)window.getSize().x / rays.size();
    int center_stripe = rays.size() / 2;
    for (int i = 0; i < rays.size(); i++) {
        if (rays[i].hasTouch) {
            // вычисление высоты
            float beta = std::abs(rays[i].angle - player.GetRotation());
            float correct_distance = rays[i].distance * std::cos(beta);
            float stripe_height = (viewportHeight * 1.5) / correct_distance;

            // вычисление позиции
            sf::Vector2f stripePosition(i * stripe_width, window.getSize().y / 2);

            // вычисление шейдера
            float shade_c = rays[i].castType == ctHorizontal ? 255 - 20 : 255;
            /*color_transform = shade_c - shade_c/rays[i].distance;
            if (rays[i].distance > 20)
                shade_c = 0;*/
            sf::Color shade(shade_c, shade_c, shade_c);
            if (i == center_stripe)
                shade = sf::Color::Red;

            // вычисление координат текстуры
            sf::Vector2f touchRelative = (rays[i].touchCoord - rays[i].cellCoord);
            float cellTextureOffset = (1 - (rays[i].castType == ctHorizontal ? touchRelative.x : touchRelative.y)); // "1 - ..." нужен так как иначе текстуры повёрнуты не в ту сторону
            int textureOffset = cellTextureOffset * wallTexture.getSize().x + 1; // 1 нужен так как текстура почему-то смещается вправо
            sf::IntRect textureRect(textureOffset, 0, 1, (int)wallTexture.getSize().y);

            // создание и отрисовка объекта
            sf::RectangleShape stripe(sf::Vector2f(stripe_width, stripe_height));
            stripe.setOrigin(stripe.getSize().x / 2, stripe.getSize().y / 2);
            stripe.setPosition(stripePosition);
            stripe.setFillColor(shade);
            stripe.setTexture(&wallTexture);
            stripe.setTextureRect(textureRect);
            window.draw(stripe);
        }
    }
}

void Renderer::Render(sf::RenderWindow& window, Player& player)
{
    const int COLUMN_COUNT = window.getSize().x;
    const int FOV = 60*DEG_TO_RAD;
    const float HALF_CLOUMN_COUNT = COLUMN_COUNT / 2.f;
    const float PROJECTION_DIST = HALF_CLOUMN_COUNT / std::tan(FOV / 2.f);
    std::vector<RayResult> rays;
    // for vr projection
    //for (float i = player.GetRotation() + 20 * DEG_TO_RAD; i >= player.GetRotation() - 20 * DEG_TO_RAD; i -= 0.25 * DEG_TO_RAD) {
    // for plane projection
    for (int i = COLUMN_COUNT; i > 0; i--) {
        float angle = std::atan((float)(i - HALF_CLOUMN_COUNT) / PROJECTION_DIST);
        RayResult ray = RunRayCast(player.GetCoord(), player.GetRotation() + angle);
        rays.push_back(ray);
    }

    RenderWorld(window, player, rays);
    RenderDebug(window, player, rays);
}
