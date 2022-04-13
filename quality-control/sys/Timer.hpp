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

inline auto currentTime()
{
    return std::chrono::high_resolution_clock::now();
}

/**
 * Timer for detecting projectile intervals
 */
struct Timer
{
    Timer() { Reset(); }
    
    void Reset() { startTime = currentTime(); }
    double GetElapsedTimeInSeconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime() - startTime).count() / 1000000000.0;
    }
    
    std::chrono::time_point<std::chrono::steady_clock> startTime;
};
    
#endif /* ProjectileManager_hpp */
