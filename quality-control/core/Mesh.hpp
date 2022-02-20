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

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    std::string type;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    
    void Draw();
private:
    GLuint VAO, VBO, EBO;
    
    void SetupMesh();
};

#endif /* Mesh_hpp */
