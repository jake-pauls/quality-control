// quality-control
// Skybox.cpp
// 2022-03-28

#include "Obj-C-Utils-Interface.h"

#include "Skybox.hpp"
#include "Assert.hpp"

Skybox::Skybox(Shader* shaderProgram, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    : GameObject(shaderProgram), _viewMatrix(viewMatrix), _projectionMatrix(projectionMatrix)
{
   _skyboxTextureId = RetrieveObjectiveCSkyboxData();
}

void Skybox::Awake()
{
    this->mesh = &Renderer::Mesh_Cube;
}

void Skybox::Draw()
{
    GL_CALL(glDepthFunc(GL_LEQUAL));
    GL_CALL(glCullFace(GL_FRONT));
    
    this->shader->Bind();
    
    // Re-create the view matrix so the skybox points where we want
    // Yes, this is a hack - we tell the skybox where to look
    // Note: If the position is set too low it could re-render the game
    glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(
        glm::vec3(10, 7, 11),
        glm::vec3(0, 7, 0),
        glm::vec3(0, 1, 0)
    )));
    
    this->shader->SetUniformMatrix4fv("_viewMatrix", glm::value_ptr(view));
    this->shader->SetUniformMatrix4fv("_projectionMatrix", glm::value_ptr(_projectionMatrix));
    
    // Bind Textures
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTextureId));
    
    this->mesh->BasicDraw();
    
    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glDepthFunc(GL_LESS));
}

void Skybox::Update()
{ }
