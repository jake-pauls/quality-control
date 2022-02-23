// quality-control
// ViewRenderer.h
// 2022-02-18

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>

#include "Obj-C-Utils-Interface.h"

@interface ViewRenderer : NSObject

@property float score;

- (void)setup:(GLKView*)view;
- (void)awake;
- (void)update;
- (void)draw;
- (void)updateScore:(float)newScore;
    
// Testing input handling
- (void)handleInput:(int) keyCode;

// Called from C++ to load shader files
+ (const char*)RetrieveFilePathByName:(const char*)fileName;

@end

#endif /* ViewRenderer_h */
