// quality-control
// Game.cpp
// 2022-02-19

#include "Game.hpp"
#include "Renderer.hpp"
#include "Assert.hpp"

#include "Cube.hpp"

Game::Game()
{ }

Game::Game(GLfloat viewWidth, GLfloat viewHeight) :
    _viewWidth(viewWidth), _viewHeight(viewHeight)
{ }

/**
 * Sets the default aspect ratio for the viewport "camera"
 * projectionMatrix defines the FOV and display range of the "camera"
 * viewMatrix defines where the "camera" is/looks at in world space
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
    
    InitializeGameObjects();
}

/**
 * Setup GameObjects that exist at the beginning of the game
 */
void Game::InitializeGameObjects()
{
    g_GameObjects.insert(new Cube());
}

/**
 * Awakens all game objects when GL sets up
 * For all GameObjects in scene, GameObject::Awake() is called
 */
void Game::Awake()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
        (*obj)->Awake();
}

/**
 * Render all game objects
 * For all GameObjects in scene, GameObject::Draw() is called
 */
void Game::Render()
{
    renderer.Clear();
    
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
        (*obj)->Draw();
}

/**
 * Updates all game objects
 * For all GameObjects in scene, GameObject::Update() is called
 */
void Game::Update()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++) {
        (*obj)->Update();
        
        // Only recalculate this matrix if the transform changes
        if ((*obj)->transform.IsModelMatrixUpdated()) {
            (*obj)->SetObjectMVPMatrix(projectionMatrix * viewMatrix * (*obj)->transform.GetModelMatrix());
        }
    }
}
