// quality-control
// ViewRenderer.mm
// 2022-02-18

#import <Foundation/Foundation.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>

#import "ViewRenderer.h"

#include "Renderer.hpp"
#include "Game.hpp"
#include "Shader.hpp"
#include "Assert.hpp"

#include "Cube.hpp"

enum
{
    UNIFORM_MVP_MATRIX,
    NUM_UNIFORMS,
};

@interface ViewRenderer() {
    GLKView* _view;
    Game game;
    Cube cube;
    
    glm::mat4 _modelViewMatrix;
    GLint uniforms[NUM_UNIFORMS];
    
    // TODO: Refactor please
    std::chrono::time_point<std::chrono::steady_clock> lastTime;
    Renderer renderer;
    float *vertices, *normals, *texCoords;
    int *indices, numberOfIndices;
}

@end

@implementation ViewRenderer

// TODO: Refactor please
@synthesize isRotating;
@synthesize rotAngle;

- (void)load
{
    numberOfIndices = renderer.drawCube(1.0f, &vertices, &normals, &texCoords, &indices);
}

/**
 * Sets up OpenGLES context and extract data from view
 */
- (void)setup:(GLKView*) view
{
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ASSERT(view.context);
    
    [EAGLContext setCurrentContext:view.context];
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    _view = view;
    
    // Create game context
    Game gameInstance(_view.bounds.size.width, _view.bounds.size.height);
    game = gameInstance;
    game.Init();
    
    // Setup shaders
    ASSERT([self setupShaders]);
    
    GL_CALL(glClearColor(0, 0, 0, 0));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glEnable(GL_BLEND));
    
    rotAngle = 0.0f;
    isRotating = 1;
    lastTime = std::chrono::steady_clock::now();
}

- (void)update
{
    cube.Update();
    
    // TODO: Refactor rotation logic
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    if (isRotating)
    {
        rotAngle += 0.001f * elapsedTime;
        
        if (rotAngle >= 360.0f)
            rotAngle = 0.0f;
    }
    
    // Update MVP matrix
    _modelViewMatrix = glm::rotate(rotAngle, glm::vec3(1.0, 0.0, 1.0));
    _modelViewMatrix = game.projectionMatrix * game.viewMatrix * _modelViewMatrix;
}

- (void)draw
{
    cube.Draw();
    
    GL_CALL(glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, &_modelViewMatrix[0][0]));
    
    // Clean viewport
    GL_CALL(glViewport(0, 0, (int) _view.drawableWidth, (int) _view.drawableHeight));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    // Setup vertex attributes
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices));
    GL_CALL(glEnableVertexAttribArray(0));
    
    GL_CALL(glVertexAttrib4f(1, 0.0f, 1.0f, 0.0f, 1.0f));
    
    GL_CALL(glUniformMatrix4fv(uniforms[UNIFORM_MVP_MATRIX], 1, GL_FALSE, &_modelViewMatrix[0][0]));
    GL_CALL(glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, indices));
}

- (bool)setupShaders
{
    Shader::ProgramSource shaderSource;
    shaderSource.vertexSource = Shader().parseShader([self retrieveFilePathByName:"Shader.vsh"]).vertexSource;
    shaderSource.fragmentSource = Shader().parseShader([self retrieveFilePathByName:"Shader.fsh"]).fragmentSource;

    GLuint programObject = Shader().createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    GL_CALL(glUseProgram(programObject));
    ASSERT(programObject != 0)

    uniforms[UNIFORM_MVP_MATRIX] = glGetUniformLocation(programObject, "modelViewProjectionMatrix");

    return true;
}

- (const char*)retrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

@end
