// quality-control
// Game.cpp
// 2022-02-19

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "Obj-C-Utils-Interface.h"

#include "Game.hpp"
#include "Assert.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Platform.hpp"
#include "Projectile.hpp"

Game::Game()
{ }

Game::Game(GLfloat viewWidth, GLfloat viewHeight) :
    _viewWidth(viewWidth), _viewHeight(viewHeight), _gameScore(0)
{ }

/**
 * Sets the default aspect ratio for the viewport "camera"
 * projectionMatrix defines the FOV and display range of the "camera"
 * viewMatrix defines where the "camera" is/looks at in world space
 */
void Game::Init()
{
    srand(time(NULL));
    
    float aspectRatio = _viewWidth / _viewHeight;
    
    ProjectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 20.0f);
    
    ViewMatrix = glm::lookAt(
        glm::vec3(2, 7, 11),
        glm::vec3(0, 2, 0),
        glm::vec3(0, 1, 0)
    );
    
    // Setup the flat shader program for textured models
    _modelLightingShaderProgram = new Shader(RetrieveObjectiveCPath("ModelLighting.vsh"), RetrieveObjectiveCPath("ModelLighting.fsh"));
    _modelLightingShaderProgram->Bind();
    
    // Setup default shader for basic lighting across game objects
    _passthroughShaderProgram = new Shader(RetrieveObjectiveCPath("Passthrough.vsh"), RetrieveObjectiveCPath("Passthrough.fsh"));
    _passthroughShaderProgram->Bind();
    
    InitializeGameObjects();
}

void Game::LoadModels()
{
    Renderer::LoadModelData();
}

/**
 * Setup GameObjects that exist at the beginning of the game
 */
void Game::InitializeGameObjects()
{
    // Start the projectile timer
    _projectileTimer.Reset();
   
    // Create the base platform
    g_GameObjects.insert(new Platform(_modelLightingShaderProgram, glm::vec3(0.0f, -1.0f, 0.0f)));
    
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            g_GameObjects.insert(new Platform(_modelLightingShaderProgram, glm::vec3(i, -1.0f, j)));
        }
    }
    
    // Track a reference of the player
    PlayerRef = new Player(_modelLightingShaderProgram);
    g_GameObjects.insert(PlayerRef);
}

void Game::DetectCollisions()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        if (dynamic_cast<Projectile *>((*obj)) != nullptr) {
            bool collision = GameObject::IsCollisionDetected(*PlayerRef, *(*obj));
            
            if (collision) {
                // Player was hit by this projectile
                // Perform some game logic
                DestroyGameObject(*(*obj));
                _gameScore--;
                break;
            } else {
                // No collisions detected
                // Check if transform is outside of the screen to destroy
                // Implement other directions/bounds for this logic
                float despawnRange = 20.0f;
                
                if (abs((*obj)->transform.position.x) >= despawnRange) {
                    DestroyGameObject(*(*obj));
                    _gameScore++;
                    break;
                } else if (abs((*obj)->transform.position.z) >= despawnRange) {
                    DestroyGameObject(*(*obj));
                    _gameScore++;
                    break;
                }
            }
        }
    }
}

/**
 * Objective-C++ Trampoline to Update UI Score
 */
int Game::GetScore()
{
    return _gameScore;
}

/**
 * Destroys a reference to a GameObject within the globale GameObject set
 * As this is a pointer, it also calls the GameObject destructor
 */
void Game::DestroyGameObject(GameObject &proj)
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        if ((*obj)->id == proj.id) {
            LOG("Destroyed a GameObject with id #" << (*obj)->id);
            delete *obj;
            g_GameObjects.erase(obj);
            break;
        }
    }
}

void Game::HandleInput(int keyCode)
{
    PlayerRef->MoveCube(keyCode);
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
    Renderer.Clear();
    
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++) {
        
        // Only recalculate this matrix if the transform changes
        if ((*obj)->transform.IsModelMatrixUpdated()) {
            (*obj)->SetObjectMVPMatrix(ProjectionMatrix * ViewMatrix * (*obj)->transform.GetModelMatrix());
        }
        
        (*obj)->Draw();
    }
}

/**
 * Updates all game objects
 * For all GameObjects in scene, GameObject::Update() is called
 */
void Game::Update()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
        (*obj)->Update();
    
    // This is where game objects are detected and IMMEDIATELY destroyed
    DetectCollisions();

    if (_projectileTimer.GetElapsedTime() >= 2)
    {
        SpawnProjectiles();
        _projectileTimer.Reset();
    }
}

void Game::SpawnProjectiles()
{
    int random = rand() % 4 + 1;
    
    Projectile* projectile;
    
    switch (random) {
        case 1:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(-8, 0.5, 0), glm::vec3(1, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 2:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(0, 0.5, -8), glm::vec3(0, 0, 1));
            g_GameObjects.insert(projectile);
            break;
        case 3:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(8, 0.5, 0), glm::vec3(-1, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 4:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(0, 0.5, 8), glm::vec3(0, 0, -1));
            g_GameObjects.insert(projectile);
            break;
    }
}
