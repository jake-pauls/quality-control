// quality-control
// Player.cpp
// 2022-02-19

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Player.hpp"

/// Awake is called when the object is initialized at setup
void Player::Awake()
{
    this->model = &Renderer::Model_Character;
    
    // Correct model transform at spawn
    this->transform.rotation.y = -90.0f;
    this->transform.scale.x = 0.5f;
    this->transform.scale.y = 0.5f;
    this->transform.scale.z = 0.5f;

    this->transform.Rotate();
    this->transform.Scale();
}

/// Draw is called when the renderer processes draw calls
void Player::Draw()
{
    // Bind shader
    this->shader->Bind();
    
    // Set shader uniforms
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader->SetUniformMatrix4fv("_mvpMatrix", glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = this->transform.GetNormalMatrix();
    this->shader->SetUniformMatrix3fv("_normalMatrix", glm::value_ptr(normalMatrix));
    
    glm::mat4 modelMatrix = this->transform.GetModelMatrix();
    this->shader->SetUniformMatrix4fv("_modelViewMatrix", glm::value_ptr(modelMatrix));
    
    // Check player model
    if (Game::HighScore >= 1000) {
        this->model = &Renderer::Model_Evil_Character;
    } else if (Game::HighScore >= 500) {
        this->model = &Renderer::Model_Gold_Character;
    } else if (Game::HighScore >= 250) {
        this->model = &Renderer::Model_Purple_Character;
    } else if (Game::HighScore >= 100) {
        this->model = &Renderer::Model_Orange_Character;
    } else if (Game::HighScore <= 50) {
        this->model = &Renderer::Model_Character;
    }

    // Determine whether player was hit to assign custom ambient lighting
    if (IsHitByProjectile) {
        glm::vec4 diffuse(0.8f, 0.1f, 0.1f, 1.0f);
        this->model->Draw(shader, diffuse);
        
        // Start the hit timer
        if (!_isHitTimerOn) {
            _hitTimer.Reset();
            _isHitTimerOn = true;
        }
        
        // Reset the hit timer and draw call after 0.55 seconds
        if (_hitTimer.GetElapsedTimeInSeconds() >= 0.55) {
            _hitTimer.Reset();
            _isHitTimerOn = false;
            IsHitByProjectile = false;
        }
    } else {
        this->model->Draw(shader);
    }
    
}

/// Update is called once per frame
void Player::Update()
{
    this->transform.Rotate();
    this->transform.Translate();
    this->transform.Scale();
}

/**
 * UI Gestures are handled by Swift and "Trampolined" into C++
 */
void Player::MoveCube(int keyCode)
{
    switch (keyCode) {
        // Up
        case 0:
            if (this->transform.position.z > -2.0f) {
                this->transform.position.z -= 1.0f;
                this->transform.rotation.x = 180.0f;
            }
            break;
        // Right
        case 1:
            if (this->transform.position.x < 2.0f) {
                this->transform.position.x += 1.0f;
                this->transform.rotation.x = 90.0f;
            }
            break;
        // Down
        case 2:
            if (this->transform.position.z < 2.0f) {
                this->transform.position.z += 1.0f;
                this->transform.rotation.x = 0.0f;
            }
            break;
        // Left
        case 3:
            if (this->transform.position.x > -2.0f) {
                this->transform.position.x -= 1.0f;
                this->transform.rotation.x = -90.0f;
            }
            break;
    }
}
