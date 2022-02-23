// quality-control
// Projectile.cpp
// 2022-02-22

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Projectile.hpp"

Projectile::Projectile(glm::vec3 position, glm::vec3 direction)
    : GameObject(), _startingPosition(position), _direction(direction)
{
    this->transform.position = position;
    
    this->transform.scale.x = 0.25f;
    this->transform.scale.y = 0.25f;
    this->transform.scale.z = 0.25f;
    
    this->transform.Scale();
    this->transform.Translate();
}

void Projectile::Awake()
{
    // Set Projectile Speed
    
    // Setup Cube Mesh
    this->mesh = Renderer().ParseCubeVertexData();
    
    // Setup Cube Shader
    this->shader = Shader(RetrieveObjectiveCPath("Shader.vsh"), RetrieveObjectiveCPath("Shader.fsh"));
    
    isTranslating = true;
    
    lastTime = std::chrono::steady_clock::now();
}

void Projectile::Draw()
{
    this->shader.Bind();
    
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader.SetUniform4f("_color", 1.0f, 0.0f, 0.0f, 1.0f);
    this->shader.SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
        
    // Draw cube mesh
    this->mesh.Draw();
}

void Projectile::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
//    lastTime = currentTime;
//
//    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    
    this->transform.Translate();
    
    float vectorUpdate = 0.0005f * elapsedTime;
    
    if (isTranslating) {
        glm::vec3 vector = glm::vec3(vectorUpdate, 0, vectorUpdate) * _direction;
        this->transform.position += vector;
            
    }
}
