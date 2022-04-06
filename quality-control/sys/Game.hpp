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
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "Timer.hpp"

#define CAMERA_POSITION glm::vec3(2, 9, 13)
#define CAMERA_LOOKS_AT glm::vec3(0, 2, 0)
#define CAMERA_UP       glm::vec3(0, 1, 0)

typedef std::set<GameObject *> GameObjectSet;

class Game
{
private:
    void InitializeGameObjects();
    void DetectCollisions();
    
public:
    enum GameState {
        MENU,
        START,
        GAME_OVER
    };
    
    GameState CurrentState = GameState::MENU;
    
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
    void SetScore(int score);
    
    // Retrieve lives
    int GetLives();
    void SetLives(int lives);
    
    // Reset wave data
    void ResetWaves();
    void KillProjectiles();
    
    void HandleInput(int keyCode);
    void SpawnProjectiles();

private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;
    
    Shader* _passthroughShaderProgram;
    Shader* _modelLightingShaderProgram;
    Shader* _skyboxShaderProgram;
   
    int _gameScore;
    int _gameLives;
    
    int _wave;
    float _speed;
    float _projectileCount;
    Timer _projectileTimer;
    
    Skybox* _skybox;
    
public:
    Renderer Renderer;
    
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    
    // Player test
    Player* PlayerRef;
    
    bool bulletFired = false;
    bool playerHit = false;
};

#endif /* Game_hpp */
