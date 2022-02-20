// quality-control
// GameObject.hpp
// 2022-02-18

#ifndef GameObject_hpp
#define GameObject_hpp

#include <OpenGLES/ES3/gl.h>

class GameObject
{
public:
    // Model, Texture, etc.
    // Transform transform;
    
public:
    virtual void Draw();
    virtual void Update();
    
protected:
    float *_vertices, *_normals, *_textureCoords;
    GLuint _indices, _numberOfIndices;
};

#endif /* GameObject_hpp */
