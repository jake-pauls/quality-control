// quality-control
// Platform.hpp
// 2022-02-20

#ifndef Platform_hpp
#define Platform_hpp

#include "GameObject.hpp"

class Platform : public GameObject
{
public:
    using GameObject::GameObject;
    
    // GameObject specific constructor
    Platform(Shader* shaderProgram, glm::vec3 position);
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
    // Set default platform model
    void SetDefaultPlatformModel();
    
    bool IsOnProjectilePath;
    
private:
    glm::vec3 _startingPosition;
    
    Model* _defaultModel;
};


#endif /* Platform_hpp */
