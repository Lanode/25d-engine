#pragma once
#include <array>
#include <vector>
#include <filesystem>
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

};

class Entity
{

};

class Map
{
    std::vector<std::vector<int>> cells;
public:
    void LoadFromFile(std::filesystem::path path);
    void GetCell(int x, int y);
};
