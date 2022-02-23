// quality-control
// GameObject.hpp
// 2022-02-18

#ifndef GameObject_hpp
#define GameObject_hpp

#include <OpenGLES/ES3/gl.h>
#include <glm/glm.hpp>

#include "Obj-C-Utils-Interface.h"

#include "Renderer.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

static int g_GameObjectIdCount = 0;

class GameObject
{
public:
    int id;
    Transform transform;
    Mesh mesh;
    Shader shader;
    
public:
    GameObject();
    
    void SetObjectMVPMatrix(glm::mat4 mvpMatrix);
    
    static bool IsCollisionDetected(GameObject &one, GameObject &two);
    
    virtual void Awake();
    virtual void Draw();
    virtual void Update();
    
protected:
    glm::mat4 _mvpMatrix;
};

#endif /* GameObject_hpp */
