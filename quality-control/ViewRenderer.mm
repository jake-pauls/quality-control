// quality-control
// ViewRenderer.mm
// 2022-02-18

#import <Foundation/Foundation.h>

#include <iostream>
#include <chrono>
#include <vector>

#import "ViewRenderer.h"

#include "Cube.hpp"
#include "Game.hpp"
#include "Assert.hpp"

@interface ViewRenderer() {
    GLKView* _viewport;
    
    Game _game;
}

@end

@implementation ViewRenderer

@synthesize gameScore;

/**
 * Sets up OpenGLES context with default settings
 * Extracts data from GLKView
 */
- (void)setup:(GLKView*) view
{
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    ASSERT(view.context);
    
    [EAGLContext setCurrentContext:view.context];
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    _viewport = view;
    
    GL_CALL(glClearColor(0, 0, 0, 0));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    
    gameScore = 0;
}

- (void)loadModels
{
    _game.LoadModels();
}
    
// MARK: Lifecycle Methods (Awake, Draw, Update)

- (void)awake
{
    _game = Game(_viewport.bounds.size.width, _viewport.bounds.size.height);
    _game.Init();
    
    _game.Awake();
}

- (void)draw
{
    _game.Render();
}

- (void)update
{
    _game.renderer.drawableWidth = _viewport.drawableWidth;
    _game.renderer.drawableHeight = _viewport.drawableHeight;
    
    _game.Update();
    gameScore = _game.GetScore();
}

/**
 * Passes Swift input to game for handling inside C++
 */
- (void)handleInput:(int)keyCode
{
    _game.HandleInput(keyCode);
}

/**
 * Retrieves a file path within the Xcode project using NSStrings
 */
+ (const char*)RetrieveFilePathByName:(const char*)fileName
{
    return [[[NSBundle mainBundle] pathForResource:[[NSString stringWithUTF8String:fileName] stringByDeletingPathExtension] ofType:[[NSString stringWithUTF8String:fileName] pathExtension]] cStringUsingEncoding:1];
}

/**
 * "Trampoline" C function called by C++ to retrieve Objective-C++ file path
 */
const char* RetrieveObjectiveCPath(const char* fileName)
{
    return [ViewRenderer RetrieveFilePathByName: fileName];
}

@end
