// quality-control
// GameObject.cpp
// 2022-02-18

#include <iostream>

#include "GameObject.hpp"
#include "Game.hpp"
#include "Assert.hpp"

GameObject::GameObject(Shader* shaderProgram)
    : shader(shaderProgram)
{
    id = g_GameObjectIdCount++;
    LOG("CREATED GAMEOBJECT WITH ID " << id);
}

void GameObject::SetObjectMVPMatrix(glm::mat4 mvpMatrix)
{
    this->_mvpMatrix = mvpMatrix;
}

bool GameObject::IsCollisionDetected(GameObject &one, GameObject &two)
{
    bool collisionX = one.transform.position.x + one.transform.scale.x >= two.transform.position.x && two.transform.position.x + two.transform.scale.x >= one.transform.position.x;
    
    bool collisionY = one.transform.position.y + one.transform.scale.y >= two.transform.position.y && two.transform.position.y + two.transform.scale.y >= one.transform.position.y;
    
    bool collisionZ = one.transform.position.z + one.transform.scale.z >= two.transform.position.z && two.transform.position.z + two.transform.scale.z >= one.transform.position.z;
    
    return collisionX && collisionY && collisionZ;
}

void GameObject::Awake()
{ }

void GameObject::Draw()
{ }

void GameObject::Update()
{ }
