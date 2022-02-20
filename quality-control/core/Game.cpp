// quality-control
// Game.cpp
// 2022-02-19

#include "Game.hpp"

#include <iostream>

Game::Game() {}

Game::Game(GLfloat viewWidth, GLfloat viewHeight) : _viewWidth(viewWidth), _viewHeight(viewHeight) {}

// TODO: Document this
void Game::Init()
{
    float aspectRatio = _viewWidth / _viewHeight;
    projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    viewMatrix = glm::lookAt(glm::vec3(-4, 3, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Game::Update(GLfloat deltaTime)
{
    // Update all game objects
    // For all gameObjects in scene, call gameObject.Update()
}

void Game::Render()
{
    // Render all game objects
    // For all gameObjects in scene, call Renderer.Draw(gameObject)
}
