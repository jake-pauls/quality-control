// quality-control
// Renderer.cpp
// 2022-02-18

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "CubeData.h"
#include "SkyboxData.h"
#include "Obj-C-Utils-Interface.h"

#include "Renderer.hpp"
#include "Assert.hpp"

/**
 * Clears the viewport before each draw call
 */
void Renderer::Clear()
{
    GL_CALL(glViewport(0, 0, drawableWidth, drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::LoadModelData()
{
    Renderer::Mesh_Cube = Renderer::ParseCubeVertexData();
    
    // Character Model
    Renderer::Model_Character = Model(RetrieveObjectiveCPath("Character.fbx"));
    Renderer::Model_Orange_Character = Model(RetrieveObjectiveCPath("CharacterOrange.fbx"));
    Renderer::Model_Purple_Character = Model(RetrieveObjectiveCPath("CharacterPurple.fbx"));
    Renderer::Model_Gold_Character = Model(RetrieveObjectiveCPath("CharacterGold.fbx"));
    Renderer::Model_Evil_Character = Model(RetrieveObjectiveCPath("CharacterEvil.fbx"));
    
    // Cubes/Platforms
    Renderer::Model_Cube_DarkGrass = Model(RetrieveObjectiveCPath("DarkGrass.obj"));
    Renderer::Model_Cube_XDarkGrass = Model(RetrieveObjectiveCPath("XDarkGrass.obj"));
    Renderer::Model_Cube_LightGrass = Model(RetrieveObjectiveCPath("LightGrass.obj"));
    Renderer::Model_Cube_XLightGrass = Model(RetrieveObjectiveCPath("XLightGrass.obj"));
    
    // Projectiles
    Renderer::Model_Projectile_SpikyBall = Model(RetrieveObjectiveCPath("SpikyBall.obj"));
    Renderer::Model_Projectile_Enemy = Model(RetrieveObjectiveCPath("Enemy.obj"));
    Renderer::Model_Projectile_Bee = Model(RetrieveObjectiveCPath("Bee.obj"));
    Renderer::Model_Projectile_Crab = Model(RetrieveObjectiveCPath("Crab.obj"));
    Renderer::Model_Projectile_Saw = Model(RetrieveObjectiveCPath("Hazard_Saw.obj"));
    Renderer::Model_Projectile_Bomb = Model(RetrieveObjectiveCPath("Bomb.obj"));
    Renderer::Model_Projectile_Pipe = Model(RetrieveObjectiveCPath("Pipe_End.obj"));
    
    // Pickups
    Renderer::Model_Pickups_Coin = Model(RetrieveObjectiveCPath("Coin.obj"));
}

/**
 * Uses the global cube data above to parse cube data into a mesh
 * Basic geometry used for testing purposes only
 */
Mesh Renderer::ParseCubeVertexData()
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Mesh::Texture> textures;
    
    int stride3 = 0;
    int stride2 = 0;
    for (int i = 0; i < NumberOfCubeVertices; i++)
    {
        Mesh::Vertex vertex;
        glm::vec3 vector;
        
        stride3 = i * 3;
        stride2 = i * 2;
        
        // Position data
        vector.x = CubeVertices[stride3];
        vector.y = CubeVertices[stride3+1];
        vector.z = CubeVertices[stride3+2];
        vertex.Position = vector;
        
        // Normal data
        vector.x = CubeNormals[stride3];
        vector.y = CubeNormals[stride3+1];
        vector.z = CubeNormals[stride3+2];
        vertex.Normal = vector;
        
        // Texture data
        vector.x = CubeTexCoords[stride2];
        vector.y = CubeTexCoords[stride2+1];
        vertex.TexCoords = vector;
        
        vertices.push_back(vertex);
    }
    
    for (int i = 0; i < NumberOfCubeIndices; i++)
    {
        indices.push_back(CubeIndices[i]);
    }
    
    Mesh::Color color;
    color.diffuse.r = 0.5f;
    color.diffuse.g = 0.5f;
    color.diffuse.b = 0.5f;
    color.diffuse.a = 1.0f;
    
    return Mesh{ vertices, indices, textures, color };
}
