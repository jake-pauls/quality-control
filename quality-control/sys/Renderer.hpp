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
    
    static Mesh ParseCubeVertexData();
    
    GLfloat drawableWidth = 10.0f;
    GLfloat drawableHeight = 10.0f;
    
    inline static Mesh CubeMesh;
};

#endif /* Renderer_hpp */
