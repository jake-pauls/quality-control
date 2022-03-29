// quality-control
// Mesh.cpp
// 2022-02-19

#include "Mesh.hpp"
#include "Assert.hpp"

Mesh::Mesh()
{ }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Color colors)
    : vertices(vertices), indices(indices), textures(textures), colors(colors)
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
    
    // Normals
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) offsetof(Vertex, Normal)));
    
    // Tex Coords
    GL_CALL(glEnableVertexAttribArray(2));
    GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *) offsetof(Vertex, TexCoords)));
    
    // Vertex Tangent
    GL_CALL(glEnableVertexAttribArray(3));
    GL_CALL(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent)));
    
    // Vertex Bitangent
    GL_CALL(glEnableVertexAttribArray(4));
    GL_CALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Bitangent)));
    
    // Indices
    GL_CALL(glGenBuffers(1, &IBO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW));
    
    // Unbind VAO
    GL_CALL(glBindVertexArray(0));
}

void Mesh::BasicDraw()
{
    // Bind and draw mesh
    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
    GL_CALL(glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0));
    
    // Unbind VAO
    GL_CALL(glBindVertexArray(0));
}

void Mesh::Draw(Shader* shaderProgram)
{
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    unsigned int normalNumber = 1;
    unsigned int heightNumber = 1;
    
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        GL_CALL(glActiveTexture(GL_TEXTURE0 + i));
        
        std::string number;
        std::string name = textures[i].type;
        
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNumber++);
        } else if (name == "texture_specular") {
            number = std::to_string(specularNumber++);
        } else if (name == "texture_normal") {
            number = std::to_string(normalNumber++);
        } else if (name == "texture_height") {
            number = std::to_string(heightNumber++);
        }
        
        // Sets uniform for sampler in the form 'texture_TYPEN'
        shaderProgram->SetUniform1i((name + number).c_str(), i);
        
        GL_CALL(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }
    
    // Bind and draw mesh
    GL_CALL(glBindVertexArray(VAO));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO))
    GL_CALL(glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0));
    
    // Unbind VAO
    GL_CALL(glBindVertexArray(0));
    
    // Unbind Texture
    GL_CALL(glActiveTexture(GL_TEXTURE0));
}
