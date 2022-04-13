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
#include "Coin.hpp"

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
    
    ProjectionMatrix = glm::perspective(glm::radians(55.0f), aspectRatio, 1.0f, 100.0f);
    
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
    
    _currentWave = 1;
    _speed = 0.2;
    _projectileCount = 1;
    _coinCount = 1;
  
    // Create the base platform (5x5 grid)
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
        Projectile* proj = dynamic_cast<Projectile *>((*obj));
        Coin* coin = dynamic_cast<Coin *>((*obj));
        
        if (proj != nullptr) {
            bool collision = GameObject::IsCollisionDetected(*PlayerRef, *(*obj));
            
            if (collision) {
                // Reset projectile lanes with particular ID's based on player location
                if (proj->LaneId == 'x')
                    ResetLaneModelsWithIndex(proj->transform.position.x, 'x');
                
                if (proj->LaneId == 'z')
                    ResetLaneModelsWithIndex(proj->transform.position.z, 'z');
                
                // Player was hit by this projectile
                // Check lose condition here
                playerHit = true;
                DestroyGameObject(*(*obj));
                _gameLives--;
                
                // Set player boolean to flash model red
                PlayerRef->IsHitByProjectile = true;
                
                if (_gameLives == 0)
                    CurrentState = GameState::GAME_OVER;
                break;
            } else {
                // No collisions detected
                // Check if transform is outside of the screen to destroy
                // Implement other directions/bounds for this logic
                // Reset lane tiles
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
        
        if (coin != nullptr) {
            bool collision = GameObject::IsCollisionDetected(*PlayerRef, *(*obj));
            
            // Player walked over a coin, destroy it and increase score
            if (collision) {
                DestroyGameObject(*(*obj));
                coinCollected = true;
                _gameScore += 10;
                break;
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
 * Objective-C++ Trampoline to reset waves and projectiles
 */
void Game::ResetWaves()
{
    _currentWave = 1;
    _speed = 0.2;
    
    _projectileCount = 1;
    _coinCount = 1;
    
    _projectileTimer.Reset();
    _coinTimer.Reset();
}

void Game::KillProjectiles()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Projectile* proj = dynamic_cast<Projectile *>((*obj));
        if (proj != nullptr)
        {
            // Reset all projectile lanes
            ResetLaneModels();
            
            // Destroy any remaining projectiles after the game ends
            DestroyGameObject(*(*obj));
            break;
        }
    }
}

void Game::KillCoins()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Coin* coin = dynamic_cast<Coin *>((*obj));
        if (coin != nullptr)
        {
            DestroyGameObject(*(*obj));
            break;
        }
    }
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
    
    // Update projectiles in the scene
    UpdateProjectiles();
    
    // Update coins in the scene
    UpdateCoins();
    
    _skybox->Update();
}

// MARK: Coin Logic

void Game::UpdateCoins()
{
   if (_coinTimer.GetElapsedTimeInSeconds() >= 8)
   {
       _coinTimer.Reset();
       
       SpawnCoins();
   }
    
    // Check if coins are dead
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Coin* coin = dynamic_cast<Coin *>((*obj));
        
        if (coin != nullptr && coin->IsCoinTimeout)
        {
            DestroyGameObject(*(*obj));
            break;
        }
    }
}

void Game::SpawnCoins()
{
    int x = 5;
    int z = 5;
    int randomlocationx = 0;
    int randomlocationz = 0;
    
    Coin* coin;
    
    for (unsigned int i = 0; i < _coinCount; i++)
    {
        randomlocationx = rand() % x + (-x/2);
        randomlocationz = rand() % z + (-z/2);
        
        coin = new Coin(_modelLightingShaderProgram, glm::vec3(randomlocationx, 0.5, randomlocationz));
        g_GameObjects.insert(coin);
    }
}

// MARK: Projectile Logic

void Game::UpdateProjectiles()
{
    if (_projectileTimer.GetElapsedTimeInSeconds() >= 5)
    {
        // Reset all projectile lanes at the start of the wave
        ResetLaneModels();
        
        _projectileTimer.Reset();
        
        switch (_currentWave) {
            case 1:
                _speed += 0.2;
                break;
            case 3:
                _coinCount += 1;
                break;
            case 5:
                _speed += 0.1;
                _projectileCount += 1;
                _coinCount += 1;
                break;
            case 10:
                _speed += 0.1;
                _projectileCount += 1;
                _coinCount += 1;
                break;
            case 15:
                _speed += 0.05;
                _coinCount += 1;
                _projectileCount += 1;
                break;
            case 20:
                _speed += 0.025;
                _coinCount += 1;
                _projectileCount += 1;
            default:
                break;
        }
        
        for (int i = 0; i < _projectileCount; i++){
            SpawnProjectiles();
        }
        
        bulletFired = true;
        _currentWave += 1;
    }
}

void Game::SpawnProjectiles()
{
    int x = 5;
    int z = 5;
    int offset = 8;
    int randomSide = rand() % 4 + 1;
    int randomlocationx = rand() % x + (-x/2);
    int randomlocationz = rand() % z + (-z/2);
    
    Projectile* projectile;
    
    switch (randomSide) {
        case 1:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(-x-offset, 0.5, randomlocationz), glm::vec3(_speed, 0, 0), 'z');
            SetProjectileLaneDiscernibility(randomlocationz, 'z');
            g_GameObjects.insert(projectile);
            break;
        case 2:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(randomlocationx, 0.5, -z-offset), glm::vec3(0, 0, _speed), 'x');
            SetProjectileLaneDiscernibility(randomlocationx, 'x');
            g_GameObjects.insert(projectile);
            break;
        case 3:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(x+offset, 0.5, randomlocationz), glm::vec3(-_speed, 0, 0), 'z');
            SetProjectileLaneDiscernibility(randomlocationz, 'z');
            g_GameObjects.insert(projectile);
            break;
        case 4:
            projectile = new Projectile(_modelLightingShaderProgram, glm::vec3(randomlocationx, 0.5, z+offset), glm::vec3(0, 0, -_speed), 'x');
            SetProjectileLaneDiscernibility(randomlocationx, 'x');
            g_GameObjects.insert(projectile);
            break;
    }
}

void Game::SetProjectileLaneDiscernibility(int laneIndex, char lane)
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Platform* plat = dynamic_cast<Platform*>((*obj));
        if (plat != nullptr) {
            if (lane == 'z') {
                if (plat->transform.position.z == laneIndex)
                    plat->IsOnProjectilePath = true;
            }
            
            if (lane == 'x') {
                if (plat->transform.position.x == laneIndex)
                    plat->IsOnProjectilePath = true;
            }
        }
    }
}

void Game::ResetLaneModels()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Platform* plat = dynamic_cast<Platform*>((*obj));
        if (plat != nullptr) {
            plat->IsOnProjectilePath = false;
        }
    }
}

void Game::ResetLaneModelsWithIndex(int laneIndex, char lane)
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        Platform* plat = dynamic_cast<Platform*>((*obj));
        if (plat != nullptr) {
            if (lane == 'z') {
                if (plat->transform.position.z == laneIndex)
                    plat->IsOnProjectilePath = false;
            }
            
            if (lane == 'x') {
                if (plat->transform.position.x == laneIndex)
                    plat->IsOnProjectilePath = false;
            }
        }
    }
}
