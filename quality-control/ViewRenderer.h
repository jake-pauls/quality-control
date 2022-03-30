// quality-control
// ViewRenderer.h
// 2022-02-18

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#include "Obj-C-Utils-Interface.h"

@interface ViewRenderer : NSObject

@property int gameScore;
@property int gameLives;
@property bool isGameStarted;
@property bool isGameOver;

- (void)setup:(GLKView*)view;
- (void)loadModels;
- (void)awake;
- (void)update;
- (void)draw;
- (void)reset;
    
// Testing input handling
- (void)handleInput:(int) keyCode;

// Update score value
- (void)updateScore:(float)newScore;

// Called from C++ to load shader files
+ (const char*)RetrieveFilePathByName:(const char*)fileName;

@end

#endif /* ViewRenderer_h */
