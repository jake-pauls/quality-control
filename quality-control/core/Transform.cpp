// quality-control
// Transform.cpp
// 2022-02-18

#include <iostream>

#include "Transform.hpp"
#include "Assert.hpp"

Transform::Transform()
{
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    
    // Set the default matrices to the identity matrix
    _translationMatrix = glm::mat4(1.0f);
    _rotationMatrix = glm::mat4(1.0f);
    _scalingMatrix = glm::mat4(1.0f);
}

/// For Translate(), Rotate(), and Scale() update the transform's
/// 'position', 'rotation', and 'scale' properties to see results

void Transform::Translate()
{
    _translationMatrix = glm::translate(glm::mat4(1.0), position);
}

/**
 * Updates the transforms _rotationMatrix with the passed angles
 * Note: the 'x' and 'y' angles are reversed for an intuitive API
 */
void Transform::Rotate()
{
    _rotationMatrix = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(0.0, 1.0, 0.0));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(rotation.y), glm::vec3(1.0, 0.0, 0.0));
    _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::Scale()
{
    _scalingMatrix = glm::scale(glm::mat4(1.0), scale);
}

/**
 * Retrieves the model matrix for the current transform
 * Defaults the translation and rotation matrices to the identity if they are zero
 */
glm::mat4 Transform::GetModelMatrix()
{
    return _translationMatrix * _rotationMatrix * _scalingMatrix;
}

glm::mat3 Transform::GetNormalMatrix()
{
    return glm::inverseTranspose(glm::mat3(GetModelMatrix()));
}

/**
 * Optimization: Checks if the model matrix was updated in the last frame
 */
bool Transform::IsModelMatrixUpdated()
{
    glm::mat4 newModelMatrix = GetModelMatrix();
    
    if (newModelMatrix == _lastModelMatrix)
        return false;
    
    _lastModelMatrix = newModelMatrix;
    
    return true;
}
