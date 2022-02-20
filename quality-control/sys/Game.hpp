// quality-control
// Game.hpp
// 2022-02-19

#ifndef Game_hpp
#define Game_hpp

#include <OpenGLES/ES3/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.hpp"

class Game
{
public:
    Game();
    Game(GLfloat viewWidth, GLfloat viewHeight);
    
    void Init();
    
    // Setup for gameObjects
    void Awake();
    
    // Game Loops
    void Render();
    void Update(GLfloat deltaTime);

public:
    Renderer renderer;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    
private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;
};

#endif /* Game_hpp */
