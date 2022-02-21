// quality-control
// Cube.hpp
// 2022-02-19

#ifndef Cube_hpp
#define Cube_hpp

#include "GameObject.hpp"

class Cube : public GameObject
{
public:
    Cube();
    
    // Implement lifecycle methods
    void Awake();
    void Draw();
    void Update();
    
public:
    // Rotation testing
    float rotAngle;
    bool isRotating;
    
    // Translation testing
    bool isRight;
    bool isTranslating;
    
    // Scale testing
    bool isMovingForward;
    bool isScaling;
    
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
};

#endif /* Cube_hpp */
