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
#include "Coin.hpp"

#define CAMERA_POSITION glm::vec3(0, 9, 13)
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
    void KillCoins();
    
    void HandleInput(int keyCode);
    
    // Coin Spawning and Updating
    void UpdateCoins();
    void SpawnCoins();
    
    // Projectile Spawning and Updating
    void UpdateProjectiles();
    void SpawnProjectiles();
    
    /// Projectile lane model methods
    void SetProjectileLaneDiscernibility(int laneIndex, char laneId);
    void ResetLaneModelsWithIndex(int laneIndex, char laneId);
    
    // Resets all lane models to a default
    void ResetLaneModels();

private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;
    
    Shader* _passthroughShaderProgram;
    Shader* _modelLightingShaderProgram;
    Shader* _skyboxShaderProgram;
   
    int _gameScore;
    int _gameLives;
    
    int _currentWave;
    float _speed;
    float _projectileCount;
    float _coinCount;
    
    Timer _projectileTimer;
    Timer _coinTimer;
    
    Skybox* _skybox;
    
public:
    Renderer Renderer;
    
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    
    // Player test
    Player* PlayerRef;
    
    bool bulletFired = false;
    bool playerHit = false;
    bool coinCollected = false;
    
    inline static int HighScore;
};

#endif /* Game_hpp */
