// quality-control
// Cube.cpp
// 2022-02-19

#include <iostream>
#include <chrono>

#include "Cube.hpp"

Cube::Cube() : GameObject()
{ }

/// Awake is called when the object is initialized at setup
void Cube::Awake()
{
    // Setup Cube Mesh
    this->mesh = Renderer().ParseCubeVertexData();
    
    // Setup Cube Shader
    this->shader = Shader(RetrieveObjectiveCPath("Shader.vsh"), RetrieveObjectiveCPath("Shader.fsh"));
    
    // Game Logic
    isRotating = false;
    isTranslating = false;
    isScaling = false;
    
    lastTime = std::chrono::steady_clock::now();
}

/// Draw is called when the renderer processes draw calls
void Cube::Draw()
{
    // Bind shader
    this->shader.Bind();
    
    // Set shader uniforms
    glm::mat4 mvp = this->_mvpMatrix;
    this->shader.SetUniform4f("_color", 0.0f, 1.0f, 0.0f, 1.0f);
    this->shader.SetUniformMatrix4fv("_mvpMatrix", &mvp[0][0]);
    
    // Draw cube mesh
    this->mesh.Draw();
    
    // Unbind shader
    this->shader.Unbind();
}

/// Update is called once per frame
void Cube::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    if (isRotating) {
        this->transform.rotation.x += 0.001f * elapsedTime;
        this->transform.rotation.y += 0.001f * elapsedTime;
        this->transform.rotation.z += 0.001f * elapsedTime;
    }
    
    if (isTranslating) {
        if (isRight) {
            this->transform.position.x += 0.001f * elapsedTime;
            this->transform.position.y += 0.001f * elapsedTime;
        } else {
            this->transform.position.x -= 0.001f * elapsedTime;
            this->transform.position.y -= 0.001f * elapsedTime;
        }
        
        if (this->transform.position.x >= 1.0f) {
            isRight = false;
        } else if (this->transform.position.x <= -1.0f) {
            isRight = true;
        }
    }
    
    if (isScaling) {
        if (isMovingForward) {
            this->transform.scale += 0.001f * elapsedTime;
        } else {
            this->transform.scale -= 0.001f * elapsedTime;
        }
        
        if (this->transform.scale.x >= 2.0f) {
            isMovingForward = false;
        } else if (this->transform.scale.x <= 0.0f) {
            isMovingForward = true;
        }
    }
    
    
    // MARK: API Examples (Rotate, Translate, Scale)
    /// Transform operations are based on the 'position', 'rotation', and 'scale' properties of the respective GameObject
     
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
            if (this->transform.position.x < 1.0f) {
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
            if (this->transform.position.x > -1.0f) {
                this->transform.position.x -= 1.0f;
            }
            break;
    }
}
