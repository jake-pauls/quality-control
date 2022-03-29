// quality-control
// Renderer.hpp
// 2022-02-18

#ifndef Renderer_hpp
#define Renderer_hpp

#include <vector>
#include <OpenGLES/ES3/gl.h>

#include "Mesh.hpp"
#include "Model.hpp"
#include "GameObject.hpp"

class Renderer
{
public:
    void Clear();
    
    static void LoadModelData();
    
    static Mesh ParseCubeVertexData();
    
    GLfloat drawableWidth = 10.0f;
    GLfloat drawableHeight = 10.0f;
    
    // Global Mesh Types
    inline static Mesh Mesh_Cube;
    
    // Global Model Types
    inline static Model Model_Character;
    inline static Model Model_Cube_Crate;
    inline static Model Model_Cube_Brick;
    inline static Model Model_Projectile_Cannonball;
    inline static Model Model_Projectile_SpikyBall;
};

#endif /* Renderer_hpp */
