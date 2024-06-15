#pragma once
#include <array>
#include <vector>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "resources.h"

// const std::array<std::array<short, 10>, 10> level = { {
//     {1,1,1,1,1,1,1,1,1,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,1,1,1,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,0,0,0,0,0,0,0,0,1},
//     {1,1,1,1,1,1,1,1,1,1}
// } };

class Material
{
public:
    sf::Texture texture;
};

class Cell
{
public:
    sf::Color color;
    std::shared_ptr<Material> wallMaterials;
};

class Scene
{
    std::vector<std::vector<std::shared_ptr<Cell>>> map;
    std::vector<std::shared_ptr<Cell>> cells;
    std::vector<std::shared_ptr<Material>> materials;
public:
    bool LoadFromResources(std::shared_ptr<ResourcesManager> resourcesManager);
    std::shared_ptr<Cell> GetCell(sf::Vector2i coord);
    sf::Vector2i GetSize();
};
