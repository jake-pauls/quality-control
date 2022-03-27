// quality-control
// Projectile.hpp
// 2022-02-22

#ifndef Projectile_hpp
#define Projectile_hpp

#include "GameObject.hpp"

class Projectile : public GameObject
{
public:
    using GameObject::GameObject;
    
    // GameObject specific constructor
    Projectile(Shader* shaderProgram, glm::vec3 position, glm::vec3 direction);
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
private:
    glm::vec3 _startingPosition;
    glm::vec3 _direction;
    
    bool isTranslating;
    
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
};

#endif /* Projectile_hpp */
