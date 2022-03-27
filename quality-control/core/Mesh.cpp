// quality-control
// Mesh.cpp
// 2022-02-19

#include "Mesh.hpp"
#include "Assert.hpp"

#include <assimp/Importer.hpp>

Mesh::Mesh()
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures)
{
    LOG("Successfully initialized new mesh.");
    SetupMesh();
}

/**
 * Sets up the vertex attributes for the mesh
 * Abuses the sequential nature of structs for a clean calculation of offsets
 */
void Mesh::SetupMesh()
{
    constexpr auto stride { sizeof(Vertex) };
    
    GL_CALL(glGenVertexArrays(1, &VAO));
    GL_CALL(glBindVertexArray(VAO));
    
    // Position Data
    GL_CALL(glGenBuffers(1, &VBO));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, &vertices[0], GL_STATIC_DRAW));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0));
    
    // Indices
    GL_CALL(glGenBuffers(1, &IBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW));
    
    // Normals
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) offsetof(Vertex, Normal)));
    
    // Tex Coords
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *) offsetof(Vertex, TexCoords)));
    
    // Unbind VAO
    GL_CALL(glBindVertexArray(0));
}

void Mesh::Draw()
{
    // TODO: Handle texture loading here, will require a reference to a Shader object
    
    // Bind and draw mesh
    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO))
    GL_CALL(glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0));
    
    // Unbind VAO
    GL_CALL(glBindVertexArray(0));
}
