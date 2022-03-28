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
    
#include "Mesh.hpp"
#include "Model.hpp"
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
    
    // Startup methods
    void Init();
    void LoadModels();
    
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
    
    Shader* _defaultShaderProgram;
    
    int _gameScore;
    int _wave;
    float _speed;
    float _projectileCount;
    Timer _projectileTimer;
    
public:
    Renderer Renderer;
    
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    
    // Player test
    Cube* Player;
    
    // Model test
    inline static Model TestModel;
};

#endif /* Game_hpp */
