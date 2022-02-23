// quality-control
// ProjectileManager.cpp
// 2022-02-23

#include "ProjectileManager.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "Assert.hpp"

#include "Platform.hpp"
#include "Projectile.hpp"

ProjectileManager::ProjectileManager()
{}


void ProjectileManager::Awake(){
    ProjectileVector = 0.1;
}

void ProjectileManager::Draw(){
     
}

void ProjectileManager::Update(){
    
}



void ProjectileManager::CreateProjectile(glm::vec3 position, glm::vec3 direction)
{
    Projectile* projectile = new Projectile(position, direction);
}
