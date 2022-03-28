// quality-control
// Projectile.cpp
// 2022-02-22

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Projectile.hpp"

Projectile::Projectile(Shader* shaderProgram, glm::vec3 position, glm::vec3 direction)
    : GameObject(shaderProgram), _startingPosition(position), _direction(direction)
{
    this->mesh = &Renderer::Mesh_Cube;
    
    // Set transform data
    this->transform.position = position;
    
    this->transform.scale.x = 0.25f;
    this->transform.scale.y = 0.25f;
    this->transform.scale.z = 0.25f;
    
    this->transform.Scale();
    this->transform.Translate();
    
    lastTime = std::chrono::steady_clock::now();
}

void Projectile::Awake()
{ }

void Projectile::Draw()
{
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader->SetUniform4f("_color", 1.0f, 0.0f, 0.0f, 1.0f);
    this->shader->SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
        
    // Draw cube mesh
//    this->mesh->Draw();
}

void Projectile::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    this->transform.Translate();
    
    float vectorUpdate = 0.01f * elapsedTime;
    
    glm::vec3 vector = glm::vec3(vectorUpdate, 0, vectorUpdate) * _direction;
    this->transform.position += vector;
}
