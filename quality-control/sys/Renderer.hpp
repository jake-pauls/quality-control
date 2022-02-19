// quality-control
// Renderer.hpp
// 2022-02-18

#ifndef Renderer_hpp
#define Renderer_hpp

#include <OpenGLES/ES3/gl.h>

class Renderer
{
public:
    int drawCube(float scale, float **vertices, float **normals, float **texCoords, int **indices);
};

#endif /* Renderer_hpp */
