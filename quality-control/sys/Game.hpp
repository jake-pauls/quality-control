// quality-control
// Game.hpp
// 2022-02-19

#ifndef Game_hpp
#define Game_hpp

#include <set>
#include <OpenGLES/ES3/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Cube.hpp"
#include "Projectile.hpp"
#include "Timer.hpp"

typedef std::set<GameObject *> GameObjectSet;

class Game
{
private:
    void InitializeGameObjects();
    void DetectCollisions();
    
public:
    Game();
    Game(GLfloat viewWidth, GLfloat viewHeight);
    
    // Set of global game objects
    GameObjectSet g_GameObjects;
   
    void DestroyGameObject(GameObject &proj);
    
    void Init();
    
    // Lifecycle methods
    void Awake();
    void Render();
    void Update();
    
    // Retrieve Score Value
    int GetScore();
    
    void HandleInput(int keyCode);
    void SpawnProjectiles();

private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;
    
    int _gameScore;
    Timer _projectileTimer;
    
public:
    Renderer renderer;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    
    // Player Test
    Cube* player;
};

#endif /* Game_hpp */
