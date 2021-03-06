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
    inline static Model Model_Orange_Character;
    inline static Model Model_Purple_Character;
    inline static Model Model_Gold_Character;
    inline static Model Model_Evil_Character;
    
    inline static Model Model_Cube_XDarkGrass;
    inline static Model Model_Cube_DarkGrass;
    inline static Model Model_Cube_LightGrass;
    inline static Model Model_Cube_XLightGrass;
    
    inline static Model Model_Projectile_SpikyBall;
    inline static Model Model_Projectile_Enemy;
    inline static Model Model_Projectile_Bee;
    inline static Model Model_Projectile_Crab;
    inline static Model Model_Projectile_Saw;
    inline static Model Model_Projectile_Bomb;
    inline static Model Model_Projectile_Pipe;
    
    inline static Model Model_Pickups_Coin;
};

#endif /* Renderer_hpp */
