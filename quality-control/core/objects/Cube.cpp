// quality-control
// Cube.cpp
// 2022-02-19

#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Cube.hpp"

/// Awake is called when the object is initialized at setup
void Cube::Awake()
{
    this->model = &Renderer::Model_Character;
    
    this->transform.rotation.y = -90.0f;
    
    this->transform.Rotate();
}

/// Draw is called when the renderer processes draw calls
void Cube::Draw()
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
    
    // Draw cube mesh
    this->model->Draw(shader);
}

/// Update is called once per frame
void Cube::Update()
{
    this->transform.Rotate();
    this->transform.Translate();
    this->transform.Scale();
}

/**
 * UI Gestures are handled by Swift and "Trampolined" into C++
 */
void Cube::MoveCube(int keyCode)
{
    switch (keyCode) {
        // Up
        case 0:
            if (this->transform.position.z > -2.0f) {
                this->transform.position.z -= 1.5f;
                this->transform.rotation.x = 180.0f;
            }
            break;
        // Right
        case 1:
            if (this->transform.position.x < 2.0f) {
                this->transform.position.x += 1.5f;
                this->transform.rotation.x = 90.0f;
            }
            break;
        // Down
        case 2:
            if (this->transform.position.z < 2.0f) {
                this->transform.position.z += 1.5f;
                this->transform.rotation.x = 0.0f;
            }
            break;
        // Left
        case 3:
            if (this->transform.position.x > -2.0f) {
                this->transform.position.x -= 1.5f;
                this->transform.rotation.x = -90.0f;
            }
            break;
    }
}
