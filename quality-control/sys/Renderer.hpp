// quality-control
// Renderer.hpp
// 2022-02-18

#ifndef Renderer_hpp
#define Renderer_hpp

#include <vector>
#include <OpenGLES/ES3/gl.h>

#include "Mesh.hpp"
#include "GameObject.hpp"

class Renderer
{
public:
    void Clear();
    void Draw(GameObject* gameObject);
    
    Mesh ParseCubeVertexData();
    int AllocateMemory(float scale, float **vertices, float **normals, float **texCoords, int **indices);
    
    GLfloat drawableWidth;
    GLfloat drawableHeight;
};

#endif /* Renderer_hpp */
