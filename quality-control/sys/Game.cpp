// quality-control
// Game.cpp
// 2022-02-19

#include "Game.hpp"
#include "Renderer.hpp"
#include "Assert.hpp"

Game::Game()
{ }

Game::Game(GLfloat viewWidth, GLfloat viewHeight) :
    _viewWidth(viewWidth), _viewHeight(viewHeight)
{ }

/**
 * Sets the default aspect ratio for the viewport "camera"
 * - projectionMatrix defines the FOV and display range of the "camera"
 * - viewMatrix defines where the "camera" is/looks at in world space
 */
void Game::Init()
{
    float aspectRatio = _viewWidth / _viewHeight;
    
    projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    viewMatrix = glm::lookAt(
        glm::vec3(0, 0, 4),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
}

void Game::Awake()
{
    // Awaken all game objects when GL sets up
    // For all gameObjects in scene, call gameObject.Awake()
}

void Game::Render()
{
    // Render all game objects
    // For all gameObjects in scene, call Renderer.Draw(gameObject)
    renderer.Clear();
}

void Game::Update(GLfloat deltaTime)
{
    // Update all game objects
    // For all gameObjects in scene, call gameObject.Update()
}

