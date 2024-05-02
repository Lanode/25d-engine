#pragma once
#include <array>
#include <vector>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "include/json.hpp"

const std::array<std::array<short, 10>, 10> level = { {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
} };

class Cell
{
    sf::Texture wallTexture;
};

class Entity
{

};

class Map
{
    std::vector<std::vector<std::shared_ptr<Cell>>> cells;
public:
    void LoadFromFile(std::filesystem::path path);
    Cell GetCell(sf::Vector2i);
    sf::Vector2i GetSize();
};
