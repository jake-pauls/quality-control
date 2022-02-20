// quality-control
// ViewRenderer.mm
// 2022-02-18

#import <Foundation/Foundation.h>

#include <iostream>
#include <chrono>
#include <vector>

#import "ViewRenderer.h"

#include "Renderer.hpp"
#include "Game.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Assert.hpp"

#include "Cube.hpp"

@interface ViewRenderer() {
    GLKView* _view;
    Game game;
    Cube cube;
    Mesh cubeMesh;
    Shader shader;
    
    glm::mat4 _modelViewMatrix;
}

@end

@implementation ViewRenderer

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
    
    cube.Awake();
    cubeMesh = game.renderer.ParseCubeVertexData();
}

- (void)draw
{
    // Check if we can update the drawableWidth/Height before rendering objects
    if (game.renderer.drawableWidth != _view.drawableWidth)
        game.renderer.drawableWidth = _view.drawableWidth;
    
    if (game.renderer.drawableHeight != _view.drawableHeight)
        game.renderer.drawableHeight = _view.drawableHeight;
    
    game.Render();
    
    cube.Draw();
    
    shader.SetUniform4f("_color", 0.0f, 1.0f, 0.0f, 1.0f);
    shader.SetUniformMatrix4fv("_mvpMatrix", &_modelViewMatrix[0][0]);
    cubeMesh.Draw();
}

- (void)update
{
    // TODO: Add cube to global gameObject set
    // game.Update()
    
    cube.Update();
    
    // Only recalculate this matrix if an object actually moves in space
    if (cube.transform.IsModelMatrixUpdated()) {
        _modelViewMatrix = game.projectionMatrix * game.viewMatrix * cube.transform.GetModelMatrix();
    }
}

- (bool)setupShaders
{
    shader = Shader([self retrieveFilePathByName:"Shader.vsh"], [self retrieveFilePathByName:"Shader.fsh"]);

    return true;
}

- (const char*)retrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

@end
