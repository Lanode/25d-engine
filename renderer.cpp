#include <functional>
#include "debug.h"
#include "scene.h"
#include "math/constants.h"
#include "math/functions.h"
#include "renderer.h"

Raycaster::Raycaster(std::shared_ptr<Scene> scene)
{
    this->scene = scene;
}

bool Raycaster::RayCollided(CastType castType, sf::Vector2f touchCoord, bool lookTo0, RayResult& rayResult)
{
    int disc_x = touchCoord.x;
    int disc_y = touchCoord.y;

    // out of bounds защита
    if ((castType == ctHorizontal && disc_y - 1 < 0) ||
        (castType == ctVertical && disc_x - 1 < 0))
        return rayResult.hasTouch;

    sf::Vector2i cellCoord;
    // if look up then subtract 1
    if (castType == ctHorizontal && lookTo0)
        cellCoord = sf::Vector2i(disc_x, disc_y - 1);
    // if look left then subtract 1
    else if (castType == ctVertical && lookTo0)
        cellCoord = sf::Vector2i(disc_x - 1, disc_y);
    // если луч в 4 области окружности
    else
        cellCoord = sf::Vector2i(disc_x, disc_y);

    if (scene->GetCell(cellCoord) != nullptr) {
        rayResult.hasTouch = true;
        rayResult.cellCoord = cellCoord;
        rayResult.touchSide = (TouchSide)(2*castType+lookTo0); // look up for TouchSide comment
        int x = 1+1;
    }

    return rayResult.hasTouch;
}

bool Raycaster::IsRayOutOfBounds(sf::Vector2f touchCoord)
{
    sf::Vector2i size = scene->GetSize();
    return (0 > touchCoord.x || touchCoord.x >= size.x) ||
           (0 > touchCoord.y || touchCoord.y >= size.y);
}

RayResult Raycaster::CastRay(CastType castType, sf::Vector2f playerCoord, float angle)
{
    const int ROV = 100;
    float ap, bp; // координаты игрока
    float a, b; // полные кординаты луча
    float as, bs; // координаты луча при полноячеечных отрезках
    float an, bn; // координаты луча при вычислении первой мили
    bool look_to0; // луч направлен на ноль
    std::function<float(float, float)> trig_f; // функция вычисления катета

    if (castType == ctHorizontal) {
        ap = playerCoord.x;
        bp = playerCoord.y;
        look_to0 = (0 < angle && angle < std::numbers::pi);
        trig_f = [](float a, float x) -> float {return x / std::tan(a); };
    }
    else {
        ap = playerCoord.y;
        bp = playerCoord.x;
        look_to0 = (std::numbers::pi / 2 < angle && angle < 3 * std::numbers::pi / 2);
        trig_f = [](float a, float x) -> float {return x * std::tan(a); };
    }

    if (look_to0)
        bn = bp - std::trunc(bp);
    else
        bn = (std::trunc(bp) + 1) - bp;

    bn = look_to0 ? -bn : bn;
    an = (castType == ctHorizontal) ? -trig_f(angle, bn) : trig_f(angle, bn);

    // b = bn;
    // a = an;

    // если идёт вверх то надо отбавлять, а если вниз то прибавлять
    bs = look_to0 ? -1 : 1;
    as = (castType == ctHorizontal) ? -trig_f(angle, bs) : trig_f(angle, bs);

    RayResult rayResult;
    rayResult.angle = angle;
    rayResult.castType = castType;
    for (int i = 0; i <= ROV; i++) {
        a = an + (float)(i)*as;
        b = bn + (float)(i)*bs;
        sf::Vector2f touchCoord;
        if (castType == ctHorizontal)
            touchCoord = sf::Vector2f(ap + a, bp + b);
        else
            touchCoord = sf::Vector2f(bp + b, ap - a);
        if (IsRayOutOfBounds(touchCoord)) {
            break;
        }
        if (RayCollided(castType, touchCoord, look_to0, rayResult)) {
            rayResult.distance = std::sqrt(std::pow(a, 2) + std::pow(b, 2));
            rayResult.touchCoord = touchCoord;
            break;
        }
        // a += (castType == ctHorizontal) ? -as : as;
        // b += bs;
        // this formulas will be moved to cycle top
        // because adding is breaking a precision
        // and we therefore we cant just add this at bottom as earlier
        // now we need multiply it on top
    }

    return rayResult;
}

RayResult Raycaster::RunRaycast(sf::Vector2f playerCoord, float angle)
{
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

Renderer::Renderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager)
{
    this->window = window;
    this->resourcesManager = resourcesManager;
}

std::vector<RayResult> debugRays;

std::vector<RayResult> WorldRenderer::CastRays()
{
    const int COLUMN_COUNT = window->getSize().x;
    const float FOV = 60 * DEG_TO_RAD;
    const float HALF_CLOUMN_COUNT = COLUMN_COUNT / 2.f;
    const float PROJECTION_DIST = HALF_CLOUMN_COUNT / std::tan(FOV / 2.f);
    std::vector<RayResult> rays;
    // for vr projection
    //for (float i = player.GetRotation() + 20 * DEG_TO_RAD; i >= player.GetRotation() - 20 * DEG_TO_RAD; i -= 0.25 * DEG_TO_RAD) {
    // for plane projection
    for (int i = COLUMN_COUNT; i > 0; i--) {
        float angle = std::atan((float)(i - HALF_CLOUMN_COUNT) / PROJECTION_DIST);
        RayResult ray = raycaster.RunRaycast(ToVector2(player->GetCoord()), player->GetRotation().x + angle);
        rays.push_back(ray);
    }
    debugRays = rays;

    return rays;
}

void WorldRenderer::RenderWalls(std::vector<RayResult>& rays)
{
    const sf::Vector2u WINDOW_CENTER = window->getSize() / 2u;

    float wallHeight = 1;

    // вычисление и отрисовка полосок
    int viewportHeight = (window->getSize().x * 9.f) / 16.f; // 16:9 = x:y // нужно для того чтобы высота блоков не менялась от изменения высоты
    float stripe_width = (float)window->getSize().x / rays.size(); // width divides by rays couint for evenly fill screen
    float wallHeightPixels = viewportHeight * (wallHeight+0.5); // wall height in pixels on distance = 1
    for (int i = 0; i < rays.size(); i++) {
        if (rays[i].hasTouch) {
            // вычисление высоты
            float beta = std::abs(rays[i].angle - player->GetRotation().x);
            float correct_distance = rays[i].distance * std::cos(beta);
            float stripe_height = wallHeightPixels / correct_distance;

            // вычисление позиции
            float pitchOffset = 0; //-(1/std::tan(player->GetRotation().y));
            float zOffset = player->GetCoord().z*2*(wallHeightPixels/2)/correct_distance - (wallHeightPixels/2)/correct_distance;
            sf::Vector2f stripePosition(i * stripe_width, WINDOW_CENTER.y + zOffset + pitchOffset);

            // вычисление шейдера
            float shade_c = rays[i].castType == ctHorizontal ? 255 - 20 : 255;
            /*color_transform = shade_c - shade_c/rays[i].distance;
            if (rays[i].distance > 20)
                shade_c = 0;*/
            sf::Color shade(shade_c, shade_c, shade_c);
            // вычисление координат текстуры
            sf::Vector2u textureSize = scene->GetCell(rays[i].cellCoord)->wallMaterials->texture.getSize();
            sf::Vector2f touchRelative = (rays[i].touchCoord - sf::Vector2f(rays[i].cellCoord));
            float cellTextureOffset = rays[i].castType == ctHorizontal ? touchRelative.x : touchRelative.y;
            if (rays[i].touchSide == tsNorth || rays[i].touchSide == tsEast)
                cellTextureOffset = 1 - cellTextureOffset; // "1 - ..." нужен так как иначе текстуры повёрнуты не в ту сторону на SE
            int textureOffset = cellTextureOffset * textureSize.x; // ??? 1 нужен так как текстура почему-то смещается вправо
            sf::IntRect textureRect(textureOffset, 0, 1, (int)textureSize.y);

            // создание и отрисовка объекта
            sf::RectangleShape stripe(sf::Vector2f(stripe_width, stripe_height));
            stripe.setOrigin(0, stripe.getSize().y / 2);
            stripe.setPosition(stripePosition);
            stripe.setFillColor(shade);
            stripe.setTexture(&scene->GetCell(rays[i].cellCoord)->wallMaterials->texture);
            stripe.setTextureRect(textureRect);
            window->draw(stripe);
        }
    }
}

void WorldRenderer::RenderSkybox()
{
    const sf::Vector2u WINDOW_CENTER = window->getSize() / 2u;

    // отрисовка фона
    sf::RectangleShape sky(sf::Vector2f(window->getSize().x, WINDOW_CENTER.y));
    sky.setFillColor(sf::Color::Cyan);
    window->draw(sky);
    sf::RectangleShape ground(sf::Vector2f(window->getSize().x, WINDOW_CENTER.y));
    ground.setPosition(sf::Vector2f(0, WINDOW_CENTER.y));
    ground.setFillColor(sf::Color::Green);
    window->draw(ground);
}

void WorldRenderer::RenderSprites()
{
    // glm::mat4x4 cam;

}

WorldRenderer::WorldRenderer(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<ResourcesManager> resourcesManager,
                             std::shared_ptr<Scene> scene, std::shared_ptr<Player> player)
    : Renderer(window, resourcesManager),
      raycaster(scene)
{
    this->scene = scene;
    this->player = player;
}

void WorldRenderer::Render()
{
    if (scene == nullptr) {
        Debug::WriteLn("No scene specified in Renderer");
        return;
    }
    if (player == nullptr) {
        Debug::WriteLn("No player specified in Renderer");
        return;
    }
    if (window == nullptr) {
        Debug::WriteLn("No window specified in Renderer");
        return;
    }

    std::vector<RayResult> rays = CastRays();

    RenderSkybox();
    RenderWalls(rays);
    RenderSprites();
}

void UIRenderer::Render()
{
    const sf::Vector2u WINDOW_CENTER = window->getSize() / 2u;

    sf::RectangleShape line(sf::Vector2f(1, 20));
    line.setOrigin(0, line.getSize().y / 2);
    line.setPosition(sf::Vector2f(WINDOW_CENTER));
    line.setFillColor(sf::Color::White);
    window->draw(line,
                 sf::BlendMode(sf::BlendMode::Zero,
                               sf::BlendMode::OneMinusDstColor,
                               sf::BlendMode::Add));
    line.rotate(90);
    window->draw(line,
                 sf::BlendMode(sf::BlendMode::Zero,
                               sf::BlendMode::OneMinusDstColor,
                               sf::BlendMode::Add));
}

void DebugRenderer::Render()
{
    const int SCALE = window->getSize().x / 40;

    if (this->scene) {
        sf::Vector2i size = scene->GetSize();
        for (int x = 0; x < size.x; x++) {
            for (int y = 0; y < size.y; y++) {
                sf::RectangleShape cell(sf::Vector2f(SCALE, SCALE));
                cell.setPosition(x * SCALE, y * SCALE);
                cell.setOutlineThickness(-1.f);
                cell.setOutlineColor(sf::Color(135, 135, 135));
                if (scene->GetCell(sf::Vector2i(x, y)) != nullptr)
                    cell.setFillColor(sf::Color(75, 75, 75));
                else
                    cell.setFillColor(sf::Color::White);
                window->draw(cell);
            }
        }
    }

    if (this->player) {
        sf::CircleShape playerShape(SCALE / 10);
        playerShape.setOrigin(playerShape.getRadius() / 2, playerShape.getRadius() / 2);
        playerShape.setPosition(ToVector2(player->GetCoord()) * (float)SCALE);
        playerShape.setFillColor(sf::Color::Red);
        window->draw(playerShape);
    }

    for (int i = 0; i < debugRays.size(); i++) {
        if (debugRays[i].hasTouch) {
            sf::RectangleShape ray(sf::Vector2f(debugRays[i].distance * SCALE, SCALE / 10));
            ray.setOrigin(ray.getSize().y / 2, 0);
            ray.setPosition(ToVector2(player->GetCoord()) * (float)SCALE);
            ray.setRotation(-debugRays[i].angle * RAD_TO_DEG);
            ray.setFillColor(sf::Color::Green);
            window->draw(ray);
        }
    }

    std::vector<uint8_t> fontBin = resourcesManager->GetResource("/res/fonts/ModernDOS8x8.ttf");
    sf::Font font;
    if (font.loadFromMemory(fontBin.data(), fontBin.size())) {
        sf::Text text;
        text.setFont(font);
        text.setString(Debug::GetMetricsAsText());
        text.setCharacterSize(15);
        text.setPosition(0, -5);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(-1);
        window->draw(text);
    }
}

void DebugRenderer::SetScene(std::shared_ptr<Scene> scene)
{
    this->scene = scene;
}

void DebugRenderer::SetPlayer(std::shared_ptr<Player> player)
{
    this->player = player;
}
