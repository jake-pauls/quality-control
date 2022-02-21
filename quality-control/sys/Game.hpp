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

#include "Renderer.hpp"

typedef std::set<GameObject *> GameObjectSet;

class Game
{
private:
    void InitializeGameObjects();
    
public:
    Game();
    Game(GLfloat viewWidth, GLfloat viewHeight);
    
    // Static set of global game objects
    GameObjectSet g_GameObjects;
    
    void Init();
    
    // Setup for gameObjects
    void Awake();
    
    // Game Loops
    void Render();
    void Update();

private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;
    
public:
    Renderer renderer;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

#endif /* Game_hpp */
