// quality-control
// Transform.hpp
// 2022-02-18

#ifndef Transform_hpp
#define Transform_hpp

#include <glm/glm.hpp>

class Transform
{
public:
    Transform();
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    void translate();
};

#endif /* Transform_hpp */
