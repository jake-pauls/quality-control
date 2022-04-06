// quality-control
// Platform.cpp
// 2022-02-20

#include <string>

#include "Platform.hpp"
#include "Game.hpp"
#include "Assert.hpp"

Platform::Platform(Shader* shaderProgram, glm::vec3 position)
    : GameObject(shaderProgram), _startingPosition(position), IsOnProjectilePath(false)
{ }

void Platform::Awake()
{
    SetDefaultPlatformModel();
    
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
    
    if (IsOnProjectilePath)
        this->model = &Renderer::Model_Cube_Crate;
    else
        this->model = this->_defaultModel;
    
    this->model->Draw(shader);
}

void Platform::Update()
{ }

/// Stripes the platform models so it looks like a grid
void Platform::SetDefaultPlatformModel()
{
    if ((int) _startingPosition.z % 2 == 0 && (int) _startingPosition.x % 2 == 0)
    {
        this->model = &Renderer::Model_Cube_Grass;
        this->_defaultModel = &Renderer::Model_Cube_Grass;
    }
    else if(abs((int) _startingPosition.z % 2) == 1 && abs((int) _startingPosition.x % 2) == 1)
    {
        this->model = &Renderer::Model_Cube_Grass;
        this->_defaultModel = &Renderer::Model_Cube_Grass;
    }
    else
    {
        this->model = &Renderer::Model_Cube_Brick;
        this->_defaultModel = &Renderer::Model_Cube_Brick;
    }
}
