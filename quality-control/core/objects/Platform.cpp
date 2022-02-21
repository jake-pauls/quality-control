// quality-control
// Platform.cpp
// 2022-02-20

#include "Platform.hpp"
#include "Assert.hpp"

Platform::Platform()
{ }

void Platform::Awake()
{
    // Setup Cube Mesh
    this->mesh = Renderer().ParseCubeVertexData();
    
    // Setup Cube Shader
    this->shader = Shader(RetrieveObjectiveCPath("Shader.vsh"), RetrieveObjectiveCPath("Shader.fsh"));
}

void Platform::Draw()
{
    this->shader.Bind();
    
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader.SetUniform4f("_color", 1.0f, 0.0f, 0.5f, 1.0f);
    this->shader.SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
    
    // Draw cube mesh
    this->mesh.Draw();
}

void Platform::Update()
{
    
}
