// quality-control
// GameObject.hpp
// 2022-02-18

#ifndef GameObject_hpp
#define GameObject_hpp

#include "Transform.hpp"

class GameObject
{
public:
    // Model, Texture, etc.
    Transform transform;
    
public:
    void draw();
    virtual void update();
};

#endif /* GameObject_hpp */
