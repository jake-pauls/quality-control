// quality-control
// ViewRenderer.mm
// 2022-02-18

#import <Foundation/Foundation.h>

#include <iostream>
#include <chrono>
#include <vector>

#import "ViewRenderer.h"

#include "Player.hpp"
#include "Game.hpp"
#include "Assert.hpp"

@interface ViewRenderer() {
    GLKView* _viewport;
    
    Game _game;
    
    SystemSoundID _soundID[4];
}

@end

@implementation ViewRenderer

@synthesize gameScore;
@synthesize gameLives;
@synthesize isGameStarted;
@synthesize isGameOver;

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
    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glFrontFace(GL_CCW));
    
    gameScore = 0;
    gameLives = 3;
    isGameOver = false;
    isGameStarted = false;
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
    
    NSString *soundFile = [[NSBundle mainBundle] pathForResource:@"gunfire" ofType:@"wav"];
    AudioServicesCreateSystemSoundID((__bridge  CFURLRef)[NSURL fileURLWithPath:soundFile], & _soundID[0]);
    soundFile = [[NSBundle mainBundle] pathForResource:@"movement" ofType:@"wav"];
    AudioServicesCreateSystemSoundID((__bridge  CFURLRef)[NSURL fileURLWithPath:soundFile], & _soundID[1]);
    soundFile = [[NSBundle mainBundle] pathForResource:@"playerHit" ofType:@"wav"];
    AudioServicesCreateSystemSoundID((__bridge  CFURLRef)[NSURL fileURLWithPath:soundFile], & _soundID[2]);
    soundFile = [[NSBundle mainBundle] pathForResource:@"coin" ofType:@"wav"];
    AudioServicesCreateSystemSoundID((__bridge  CFURLRef)[NSURL fileURLWithPath:soundFile], & _soundID[3]);
}

- (void)draw
{
    _game.Render();
}

- (void)update
{
    _game.Renderer.drawableWidth = _viewport.drawableWidth;
    _game.Renderer.drawableHeight = _viewport.drawableHeight;
    
    if (isGameStarted && _game.CurrentState != Game::GameState::GAME_OVER)
    {
        _game.CurrentState = Game::GameState::START;
        
        _game.Update();
        gameScore = _game.GetScore();
        gameLives = _game.GetLives();
        
        //use this for when calling bullet sound
        if (_game.bulletFired) {
            [self activateSFX:0];
            _game.bulletFired = false;
        }
        if (_game.playerHit) {
            [self activateSFX:2];
            _game.playerHit = false;
        }
        if (_game.coinCollected) {
            [self activateSFX:3];
            _game.coinCollected = false;
        }
    }
    
    if (_game.CurrentState == Game::GameState::GAME_OVER)
    {
        isGameStarted = false;
        isGameOver = true;
        
        _game.KillProjectiles();
        _game.KillCoins();
    }
}

- (void)reset
{
    LOG("[Lifecycle] The game is being reset");
    
    // Reset score and lives
    _game.SetScore(0);
    _game.SetLives(3);
    
    // Reset player position
    _game.PlayerRef->transform.position.x = 0.0f;
    _game.PlayerRef->transform.position.y = 0.0f;
    _game.PlayerRef->transform.position.z = 0.0f;
    
    // Reset waves
    _game.ResetWaves();
    
    // Reset Game State
    _game.CurrentState = Game::GameState::START;
    isGameOver = false;
    isGameStarted = true;
}

- (void)activateSFX:(int)index
{
    AudioServicesPlaySystemSound(_soundID[index]);
}

/**
 * Passes Swift input to game for handling inside C++
 */
- (void)handleInput:(int)keyCode
{
    _game.HandleInput(keyCode);
    [self activateSFX:1];
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
