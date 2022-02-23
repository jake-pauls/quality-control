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
    GLKView* _view;
    Game game;
}

@end

@implementation ViewRenderer
@synthesize score;
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
    _view = view;
    
    GL_CALL(glClearColor(0, 0, 0, 0));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glEnable(GL_BLEND));
    
    score = 0;
}

// MARK: Lifecycle Methods (Awake, Draw, Update)

- (void)awake
{
    game = Game(_view.bounds.size.width, _view.bounds.size.height);
    game.Init();
    
    game.Awake();
}

- (void)draw
{
    // Check if we can update the drawableWidth/Height before rendering objects
    if (game.renderer.drawableWidth != _view.drawableWidth)
        game.renderer.drawableWidth = _view.drawableWidth;
    
    if (game.renderer.drawableHeight != _view.drawableHeight)
        game.renderer.drawableHeight = _view.drawableHeight;
    
    game.Render();
}

- (void)update
{
    game.Update();
    score = game.GetScore();
    printf("%d", game.GetScore());
}

/**
 * Passes Swift input to game for handling inside C++
 */
- (void)handleInput:(int)keyCode
{
    game.HandleInput(keyCode);
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
