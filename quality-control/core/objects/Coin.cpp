// quality-control
// Coin.cpp
// 2022-04-12

#include "Coin.hpp"
#include "Assert.hpp"

Coin::Coin(Shader* shaderProgram, glm::vec3 position)
    : GameObject(shaderProgram), _startingPosition(position)
{
    this->model = &Renderer::Model_Pickups_Coin;
    
    // Set transform data
    this->transform.position = position;
    
    this->transform.scale.x = 0.4f;
    this->transform.scale.y = 0.4f;
    this->transform.scale.z = 0.4f;
    
    this->transform.Scale();
    this->transform.Translate();
    
    lastTime = std::chrono::steady_clock::now();
}

void Coin::Awake()
{ }

void Coin::Draw()
{
    this->shader->Bind();
    
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader->SetUniformMatrix4fv("_mvpMatrix", glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = this->transform.GetNormalMatrix();
    this->shader->SetUniformMatrix3fv("_normalMatrix", glm::value_ptr(normalMatrix));
    
    glm::mat4 modelMatrix = this->transform.GetModelMatrix();
    this->shader->SetUniformMatrix4fv("_modelViewMatrix", glm::value_ptr(modelMatrix));
    
    if (_coinTimer.GetElapsedTimeInSeconds() >= 5) {
        glm::vec4 diffuse(1.0f, 0.2f, 0.2f, 1.0f);
        this->model->Draw(shader, diffuse);
    } else {
        this->model->Draw(shader);
    }
}

void Coin::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    float rotationUpdate = 0.15f * elapsedTime;
    
    glm::vec3 rot(rotationUpdate, 0.0f, 0.0f);
    this->transform.rotation += rot;
    this->transform.Rotate();
    
    // After 6 seconds, add the coin to the kill list
    if (_coinTimer.GetElapsedTimeInSeconds() >= 6)
        IsCoinTimeout = true;
}
