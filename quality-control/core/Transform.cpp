// quality-control
// Transform.cpp
// 2022-02-18

#include <iostream>

#include "Transform.hpp"

Transform::Transform()
{
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    
    // Set the default scaling matrix to the identity matrix
    _scalingMatrix = glm::mat4(1.0);
}

void Transform::Translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
{
    _translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
}

/**
 * Updates the transforms _rotationMatrix with the passed angles
 * Note: the 'x' and 'y' angles are reversed for an intuitive API
 */
void Transform::Rotate(float xAngle = 0.0f, float yAngle = 0.0f, float zAngle = 0.0f)
{
    _rotationMatrix = glm::rotate(glm::mat4(1.0), xAngle, glm::vec3(0.0, 1.0, 0.0));
    _rotationMatrix = glm::rotate(_rotationMatrix, yAngle, glm::vec3(1.0, 0.0, 0.0));
    _rotationMatrix = glm::rotate(_rotationMatrix, zAngle, glm::vec3(0.0, 0.0, 1.0));
}

void Transform::Scale(glm::vec3 scaleFactor)
{
    _scalingMatrix = glm::scale(glm::mat4(1.0), scaleFactor);
}

/**
 * Retrieves the model matrix for the current transform
 * Defaults the translation and rotation matrices to the identity if they are zero
 */
glm::mat4 Transform::GetModelMatrix()
{
    glm::mat4 zeroMatrix = glm::mat4(0.0);
    glm::mat4 identityMatrix = glm::mat4(1.0);
    
    if (_translationMatrix == zeroMatrix)
        _translationMatrix = identityMatrix;
    
    if (_rotationMatrix == zeroMatrix)
        _rotationMatrix = identityMatrix;
    
    return _translationMatrix * _rotationMatrix * _scalingMatrix;
}

/**
 * Optimization: Checks if the model matrix was updated in the last frame
 */
bool Transform::IsModelMatrixUpdated()
{
    glm::mat4 newModelMatrix = GetModelMatrix();
    
    if (newModelMatrix == _lastModelMatrix) {
        return false;
    }
    
    _lastModelMatrix = newModelMatrix;
    
    return true;
}
