// quality-control
// Cube.hpp
// 2022-02-19

#ifndef Cube_hpp
#define Cube_hpp

#include "GameObject.hpp"

class Cube : public GameObject
{
public:
    using GameObject::GameObject;
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
    void MoveCube(int keyCode);
};

#endif /* Cube_hpp */
