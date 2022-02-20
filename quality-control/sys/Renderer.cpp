// quality-control
// Renderer.cpp
// 2022-02-18

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "Renderer.hpp"
#include "Assert.hpp"

int numVertices = 24;
int numIndices = 36;

float cubeVerts[] =
{
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    0.5f,  0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
};

float cubeNormals[] =
{
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
};

float cubeTex[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

GLuint cubeIndices[] =
{
    0, 2, 1,
    0, 3, 2,
    4, 5, 6,
    4, 6, 7,
    8, 9, 10,
    8, 10, 11,
    12, 15, 14,
    12, 14, 13,
    16, 17, 18,
    16, 18, 19,
    20, 23, 22,
    20, 22, 21
};

void Renderer::Draw(GameObject* gameObject)
{ }

/**
 * Clears the viewport before each draw call
 */
void Renderer::Clear()
{
    GL_CALL(glViewport(0, 0, drawableWidth, drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

Mesh Renderer::ParseCubeVertexData()
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    int stride3 = 0;
    int stride2 = 0;
    for (int i = 0; i < numVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        
        stride3 = i * 3;
        stride2 = i * 2;
        
        // Position data
        vector.x = cubeVerts[stride3];
        vector.y = cubeVerts[stride3+1];
        vector.z = cubeVerts[stride3+2];
        vertex.Position = vector;
        
        // Normal data
        vector.x = cubeNormals[stride3];
        vector.y = cubeNormals[stride3+1];
        vector.z = cubeNormals[stride3+2];
        vertex.Normal = vector;
        
        // Texture data
        vector.x = cubeTex[stride2];
        vector.y = cubeTex[stride2+1];
        vertex.TexCoords = vector;
        
        vertices.push_back(vertex);
    }
    
    for (int i = 0; i < numIndices; i++)
    {
        indices.push_back(cubeIndices[i]);
    }
    
    return Mesh{ vertices, indices, textures };
}
