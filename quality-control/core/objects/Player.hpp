// quality-control
// Player.hpp
// 2022-02-19

#ifndef Player_hpp
#define Player_hpp

#include "GameObject.hpp"
#include "Timer.hpp"

class Player : public GameObject
{
public:
    using GameObject::GameObject;
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
    void MoveCube(int keyCode);
    
    bool IsHitByProjectile = false;
    
private:
    Timer _hitTimer;
    
    bool _isHitTimerOn = false;
};

#endif /* Player_hpp */
