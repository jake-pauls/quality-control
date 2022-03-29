// quality-control
// Skybox.hpp
// 2022-03-28

#ifndef Skybox_hpp
#define Skybox_hpp

#include "GameObject.hpp"
#include "Shader.hpp"

class Skybox : public GameObject
{
public:
    using GameObject::GameObject;
    
    // GameObject specific constructor
    Skybox(Shader* shaderProgram, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
private:
    GLuint _skyboxTextureId;
    
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;
};

#endif /* Skybox_hpp */
