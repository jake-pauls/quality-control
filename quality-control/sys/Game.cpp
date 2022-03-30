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

#include "Skybox.hpp"
#include "Platform.hpp"
#include "Projectile.hpp"

Game::Game()
{ }

Game::Game(GLfloat viewWidth, GLfloat viewHeight) :
    _viewWidth(viewWidth), _viewHeight(viewHeight), _gameScore(0), _gameLives(3)
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
    
    ProjectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 20.0f);
    
    ViewMatrix = glm::lookAt(
        CAMERA_POSITION,
        CAMERA_LOOKS_AT,
        CAMERA_UP
    );
    
    // Setup the flat shader program for textured models
    _modelLightingShaderProgram = new Shader(RetrieveObjectiveCPath("ModelLighting.vsh"), RetrieveObjectiveCPath("ModelLighting.fsh"));
    _modelLightingShaderProgram->Bind();
    
    // Setup default shader for basic lighting across game objects
    _passthroughShaderProgram = new Shader(RetrieveObjectiveCPath("Passthrough.vsh"), RetrieveObjectiveCPath("Passthrough.fsh"));
    _passthroughShaderProgram->Bind();
    
    // Setup the skybox shader program with default sampler
    _skyboxShaderProgram = new Shader(RetrieveObjectiveCPath("Skybox.vsh"), RetrieveObjectiveCPath("Skybox.fsh"));
    _skyboxShaderProgram->Bind();
    
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
    // Create the skybox
    _skybox = new Skybox(_skyboxShaderProgram, ViewMatrix, ProjectionMatrix);
  
    // Start the projectile timer
    _projectileTimer.Reset();
    
    _wave = 1;
    _speed = 0.2;
    _projectileCount = 1;
  
    // Create the base platform
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            g_GameObjects.insert(new Platform(_modelLightingShaderProgram, glm::vec3(i, -0.5f, j)));
        }
    }
    
    // Track a reference of the player
    PlayerRef = new Player(_modelLightingShaderProgram);
    g_GameObjects.insert(PlayerRef);
    
    // Start the projectile timer
    _projectileTimer.Reset();
}

void Game::DetectCollisions()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        if (dynamic_cast<Projectile *>((*obj)) != nullptr) {
            bool collision = GameObject::IsCollisionDetected(*PlayerRef, *(*obj));
            
            if (collision) {
                // Player was hit by this projectile
                // Check lose condition here
                DestroyGameObject(*(*obj));
                _gameScore--;
                _gameLives--;
                
                if (_gameLives == 0) {
                    CurrentState = GameState::GAME_OVER;
                }
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

void Game::SetScore(int score)
{
    _gameScore = score;
}

/**
 * Objective-C++ Trampoline to Update UI Lives
 */
int Game::GetLives()
{
    return _gameLives;
}

void Game::SetLives(int lives)
{
    _gameLives = lives;
}

/**
 * Objective-C++ Trampoline to reset waves
 */
void Game::ResetWaves()
{
    _wave = 1;
    _speed = 0.2;
    _projectileCount = 1;
    _projectileTimer.Reset();
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
    
    _skybox->Awake();
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
    
    _skybox->Draw();
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
    if (_projectileTimer.GetElapsedTime() >= 5)
    {
        _projectileTimer.Reset();
        
        switch (_wave) {
            case 1:
                _speed += 0.2;
                break;
            case 5:
                _speed += 0.1;
                _projectileCount += 1;
                break;
            case 10:
                _speed += 0.1;
                _projectileCount += 1;
                break;
            default:
                break;
        }
        
        for (int i = 0; i < _projectileCount; i++){
            SpawnProjectiles();
        }
        
        _wave += 1;
    }
    
    _skybox->Update();
}

void Game::SpawnProjectiles()
{
    int x = 4;
    int z = 4;
    int offset = 8;
    int randomside = rand() % 4 + 1;
    int randomlocationx = rand() % x + (-x/2);
    int randomlocationz = rand() % z + (-z/2);
    
    Projectile* projectile;
    
    switch (randomside) {
        case 1:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(-x-offset, 0.5, randomlocationz), glm::vec3(_speed, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 2:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(randomlocationx, 0.5, -z-offset), glm::vec3(0, 0, _speed));
            g_GameObjects.insert(projectile);
            break;
        case 3:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(x+offset, 0.5, randomlocationz), glm::vec3(-_speed, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 4:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(randomlocationx, 0.5, z+offset), glm::vec3(0, 0, -_speed));
            g_GameObjects.insert(projectile);
            break;
    }
}
