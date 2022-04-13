// quality-control
// Coin.hpp
// 2022-04-12

#ifndef Coin_hpp
#define Coin_hpp

#include "GameObject.hpp"
#include "Timer.hpp"

class Coin : public GameObject
{
public:
    using GameObject::GameObject;
    
    // GameObject specific constructor
    Coin(Shader* shaderProgram, glm::vec3 position);
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
    bool IsCoinTimeout = false;
    
private:
    glm::vec3 _startingPosition;
    
    Timer _coinTimer;
    
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
};

#endif /* Coin_hpp */
