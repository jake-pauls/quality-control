// quality-control
// Projectile.cpp
// 2022-02-22

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Projectile.hpp"
#include "Assert.hpp"

Projectile::Projectile(Shader* shaderProgram, glm::vec3 position, glm::vec3 direction, char laneId)
    : GameObject(shaderProgram), _startingPosition(position), _direction(direction), LaneId(laneId)
{
    // Basic logic to provide variation to projectile models (or behaviours?)
    int random = rand() % 3;
    
    if (random == 0)
        this->model = &Renderer::Model_Projectile_SpikyBall;
    else if (random == 1)
        this->model = &Renderer::Model_Projectile_Enemy;
    else
        this->model = &Renderer::Model_Projectile_Saw;
    
    // Set transform data
    this->transform.position = position;
    
    this->transform.scale.x = 0.3f;
    this->transform.scale.y = 0.3f;
    this->transform.scale.z = 0.3f;
    
    this->transform.Scale();
    this->transform.Translate();
    
    lastTime = std::chrono::steady_clock::now();
}

void Projectile::Awake()
{ }

void Projectile::Draw()
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

void Projectile::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    float vectorUpdate = 0.01f * elapsedTime;
    float rotationUpdate = 0.15f * elapsedTime;
    
    glm::vec3 vector = glm::vec3(vectorUpdate, 0, vectorUpdate) * _direction;
    this->transform.position += vector;
    this->transform.Translate();
    
    // Only rotate saws and enemies about Y-axis if they are spawned in
    vector = glm::vec3(rotationUpdate, 0, rotationUpdate) * _direction;

    this->transform.rotation += vector;
    this->transform.Rotate();
}
