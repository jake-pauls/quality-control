// quality-control
// Platform.cpp
// 2022-02-20

#include <string>
#include "Platform.hpp"
#include "Assert.hpp"

void Platform::Awake()
{
    this->mesh = &Renderer::CubeMesh;
    
    // Platform Position
    this->transform.position.x = 0.0f;
    this->transform.position.y = -2.25f;
    
    // Platform Scale
    this->transform.scale.x = 5.0f;
    this->transform.scale.z = 5.0f;
    this->transform.scale.y = 0.25f;
    
    // Apply platform transform changes before rendering
    this->transform.Translate();
    this->transform.Scale();
}

void Platform::Draw()
{
    glm::mat4 mvp = this->_mvpMatrix;
    
    this->shader->SetUniform4f("_color", 0.0f, 0.6f, 1.0f, 1.0f);
    this->shader->SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
    
    this->mesh->Draw();
}

void Platform::Update()
{ }
