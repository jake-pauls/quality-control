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
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    
    // Setup default shader for basic lighting across game objects
    _defaultShaderProgram = new Shader(RetrieveObjectiveCPath("Shader.vsh"), RetrieveObjectiveCPath("Shader.fsh"));
    _defaultShaderProgram->Bind();
    
    InitializeGameObjects();
}

void Game::LoadModels()
{
    Renderer::CubeMesh = Renderer::ParseCubeVertexData();
    
    TestModel = Model(RetrieveObjectiveCPath("Cube_Grass_Single.fbx"));
}

/**
 * Setup GameObjects that exist at the beginning of the game
 */
void Game::InitializeGameObjects()
{
    // Start the projectile timer
    _projectileTimer.Reset();
    
    _wave = 1;
    _speed = 0.2;
    _projectileCount = 1;
   
    // Create the base platform
    g_GameObjects.insert(new Platform(_defaultShaderProgram));
    
    // Track a reference of the player
    Player = new Cube(_defaultShaderProgram);
    g_GameObjects.insert(Player);
}

void Game::DetectCollisions()
{
    for (GameObjectSet::iterator obj = g_GameObjects.begin(); obj != g_GameObjects.end(); obj++)
    {
        if (dynamic_cast<Projectile *>((*obj)) != nullptr) {
            bool collision = GameObject::IsCollisionDetected(*Player, *(*obj));
            
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
    Player->MoveCube(keyCode);
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
            projectile = new Projectile(_defaultShaderProgram, glm::vec3(-x-offset, 0, randomlocationz), glm::vec3(_speed, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 2:
            projectile = new Projectile(_defaultShaderProgram, glm::vec3(randomlocationx, 0, -z-offset), glm::vec3(0, 0, _speed));
            g_GameObjects.insert(projectile);
            break;
        case 3:
            projectile = new Projectile(_defaultShaderProgram, glm::vec3(x+offset, 0, randomlocationz), glm::vec3(-_speed, 0, 0));
            g_GameObjects.insert(projectile);
            break;
        case 4:
            projectile = new Projectile(_defaultShaderProgram, glm::vec3(randomlocationx, 0, z+offset), glm::vec3(0, 0, -_speed));
            g_GameObjects.insert(projectile);
            break;
    }
}
