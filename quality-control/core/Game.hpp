// quality-control
// Game.hpp
// 2022-02-19

#ifndef Game_hpp
#define Game_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OpenGLES/ES3/gl.h>

class Game
{
public:
    Game();
    Game(GLfloat viewWidth, GLfloat viewHeight);
    
    void Init();
    
    // Game Loops
    void Update(GLfloat deltaTime);
    void Render();
    
private:
    GLfloat _viewWidth;
    GLfloat _viewHeight;

public:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

#endif /* Game_hpp */
