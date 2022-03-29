// quality-control
// Mesh.hpp
// 2022-02-19

#ifndef Mesh_hpp
#define Mesh_hpp

#include <string>
#include <vector>
#include <OpenGLES/ES3/gl.h>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture
    {
        GLuint id;
        std::string type;
        std::string path;
    };
    
    struct Color
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 emissive;
    };
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Color colors;
    
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Color colors);
    
    // No Shader, Basic Geometry with GL_TRIANGLES
    void BasicDraw();
    
    // Shader, Textures, and Model Geometry
    void Draw(Shader* shaderProgram);
    
private:
    GLuint VAO;
    GLuint VBO, IBO;
    
    void SetupMesh();
};

#endif /* Mesh_hpp */
