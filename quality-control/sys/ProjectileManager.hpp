// quality-control
// ProjectileManager.hpp
// 2022-02-23

#ifndef ProjectileManager_hpp
#define ProjectileManager_hpp

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Projectile.hpp"

class ProjectileManager		
{
public:
    ProjectileManager();
    
    void Awake();
    void Draw();
    void Update();
    void CreateProjectile(glm::vec3 position, glm::vec3 direction);

private:
    // Speed of Projectile
    float ProjectileVector;
};
    
#endif /* ProjectileManager_hpp */
