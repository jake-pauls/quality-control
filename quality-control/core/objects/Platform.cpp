// quality-control
// Platform.cpp
// 2022-02-20

#include <string>

#include "Platform.hpp"
#include "Game.hpp"
#include "Assert.hpp"

Platform::Platform(Shader* shaderProgram, glm::vec3 position)
    : GameObject(shaderProgram), _startingPosition(position)
{ }

void Platform::Awake()
{
    this->model = &Renderer::Model_Cube_Brick;
    
    // Platform Position
    this->transform.position = _startingPosition;
    this->transform.position.y = -0.5f;
    
    // Platform Scale
    this->transform.scale.x = 1.0f;
    this->transform.scale.z = 1.0f;
    this->transform.scale.y = 1.0f;
    
    // Apply platform transform changes before rendering
    this->transform.Translate();
    this->transform.Scale();
}

void Platform::Draw()
{
    this->shader->Bind();
    
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader->SetUniformMatrix4fv("_mvpMatrix", glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = this->transform.GetNormalMatrix();
    this->shader->SetUniformMatrix3fv("_normalMatrix", glm::value_ptr(normalMatrix));
    
    glm::mat4 modelMatrix = this->transform.GetModelMatrix();
    this->shader->SetUniformMatrix4fv("_modelViewMatrix", glm::value_ptr(modelMatrix));
    
    this->model->Draw(shader);
}

void Platform::Update()
{ }
