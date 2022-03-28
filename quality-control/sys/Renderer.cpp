// quality-control
// Renderer.cpp
// 2022-02-18

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "CubeData.h"

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

/**
 * Uses the global cube data above to parse cube data into a mesh
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
