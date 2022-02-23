// quality-control
// Game.cpp
// 2022-02-19

#include "Game.hpp"
#include "Renderer.hpp"
#include "Assert.hpp"

#include "Platform.hpp"
#include "Projectile.hpp"

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
        glm::vec3(2, 5, 8),
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
    // Track a reference of the player
    player = new Cube();
    g_GameObjects.insert(player);
    
    g_GameObjects.insert(new Platform());

    Projectile* projectile = new Projectile(glm::vec3(-8,0,0), glm::vec3(1,0,0));
    g_GameObjects.insert(projectile);
    g_GameObjectsProjectiles.insert(projectile);
}

void Game::DetectCollisions()
{
    for (GameObjectSet::iterator proj = g_GameObjectsProjectiles.begin(); proj != g_GameObjectsProjectiles.end(); proj++)
    {
        bool collision = GameObject::IsCollisionDetected(*player, *(*proj));
        
        if (collision) {
            // Player was hit by this projectile
            // Perform some game logic
        } else {
            // No collisions detected
            // Check if transform is outside of the screen to destroy
            // Implement other directions/bounds for this logic
            
            float despawnRange = 10.0f;
            
            if (abs((*proj)->transform.position.x) >= despawnRange ) {
                DestroyGameObject(*(*proj));
            } else if (abs((*proj)->transform.position.z) >= despawnRange) {
                DestroyGameObject(*(*proj));
            }
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
            LOG("Successfully destroyed a GameObject with id #" << (*obj)->id);
            g_GameObjects.erase(obj);
            break;
        }
    }
}

void Game::HandleInput(int keyCode)
{
    player->MoveCube(keyCode);
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
    
    
    
    DetectCollisions();
}
