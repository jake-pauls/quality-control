// quality-control
// GameObject.cpp
// 2022-02-18

#include "GameObject.hpp"
#include "Game.hpp"

void GameObject::SetObjectMVPMatrix(glm::mat4 mvpMatrix)
{
    this->_mvpMatrix = mvpMatrix;
}

void GameObject::Awake()
{ }

void GameObject::Draw()
{ }

void GameObject::Update()
{ }
