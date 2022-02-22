// quality-control
// Transform.hpp
// 2022-02-18

#ifndef Transform_hpp
#define Transform_hpp

#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

class Transform
{
public:
    Transform();
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    // API methods
    void Translate();
    void Rotate();
    void Scale();
    
    bool IsModelMatrixUpdated();
    glm::mat4 GetModelMatrix();
    
private:
    glm::mat4 _rotationMatrix;
    glm::mat4 _translationMatrix;
    glm::mat4 _scalingMatrix;
    
    // Used for comparing model matrices between frames
    glm::mat4 _lastModelMatrix;
};

#endif /* Transform_hpp */
