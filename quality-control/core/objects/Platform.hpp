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
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
};


#endif /* Platform_hpp */
