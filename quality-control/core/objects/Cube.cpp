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
    this->model = &Game::TestModel;
}

/// Draw is called when the renderer processes draw calls
void Cube::Draw()
{
    // Bind shader
    this->shader->Bind();
    
    // Set shader uniforms
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader->SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
    
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
                this->transform.position.z -= 1.0f;
            }
            break;
        // Right
        case 1:
            if (this->transform.position.x < 2.0f) {
                this->transform.position.x += 1.0f;
            }
            break;
        // Down
        case 2:
            if (this->transform.position.z < 2.0f) {
                this->transform.position.z += 1.0f;
            }
            break;
        // Left
        case 3:
            if (this->transform.position.x > -2.0f) {
                this->transform.position.x -= 1.0f;
            }
            break;
    }
}
