// quality-control
// ViewRenderer.h
// 2022-02-18

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>

#include "Obj-C-Utils-Interface.h"

@interface ViewRenderer : NSObject

- (void)setup:(GLKView*)view;
- (void)awake;
- (void)update;
- (void)draw;

// Called from C++ to load shader files
+ (const char*)RetrieveFilePathByName:(const char*)fileName;

@end

#endif /* ViewRenderer_h */
