// quality-control
// Cube.cpp
// 2022-02-19

#include <iostream>

#include "Cube.hpp"

Cube::Cube() {}

void Cube::Draw()
{
    std::cout << "Making cube draw call" << std::endl;
}

void Cube::Update()
{
    std::cout << "Making cube update call" << std::endl;
}
