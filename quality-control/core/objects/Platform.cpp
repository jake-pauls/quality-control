// quality-control
// Platform.cpp
// 2022-02-20

#include "Platform.hpp"
#include "Assert.hpp"

Platform::Platform() : GameObject()
{ }

void Platform::Awake()
{
    // Setup Cube Mesh
    this->mesh = Renderer().ParseCubeVertexData();
    
    // Setup Cube Shader
    this->shader = Shader(RetrieveObjectiveCPath("Shader.vsh"), RetrieveObjectiveCPath("Shader.fsh"));
    
    // Platform Position
    this->transform.position.x = 0.0f;
    this->transform.position.y = -2.25f;
    
    // Platform Scale
    this->transform.scale.x = 5.0f;
    this->transform.scale.z = 5.0f;
    this->transform.scale.y = 0.25f;
}

void Platform::Draw()
{
    this->shader.Bind();
    
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader.SetUniform4f("_color", 0.0f, 0.6f, 1.0f, 1.0f);
    this->shader.SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
    
    this->transform.Translate();
    this->transform.Scale();
    
    this->mesh.Draw();
}

void Platform::Update()
{ }
