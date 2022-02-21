// quality-control
// GameObject.hpp
// 2022-02-18

#ifndef GameObject_hpp
#define GameObject_hpp

#include <OpenGLES/ES3/gl.h>
#include <glm/glm.hpp>

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

class GameObject
{
public:
    Transform transform;
    Mesh mesh;
    Shader shader;
    
public:
    void SetObjectMVPMatrix(glm::mat4 mvpMatrix);
    
    virtual void Awake();
    virtual void Draw();
    virtual void Update();
    
protected:
    glm::mat4 _mvpMatrix;
};

#endif /* GameObject_hpp */
