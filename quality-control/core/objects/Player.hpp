// quality-control
// Player.hpp
// 2022-02-19

#ifndef Player_hpp
#define Player_hpp

#include "GameObject.hpp"

class Player : public GameObject
{
public:
    using GameObject::GameObject;
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
    void MoveCube(int keyCode);
};

#endif /* Player_hpp */
