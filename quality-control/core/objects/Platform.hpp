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
    
private:
    glm::vec3 _startingPosition;
};


#endif /* Platform_hpp */
