// quality-control
// ViewRenderer.h
// 2022-02-18

#ifndef ViewRenderer_h
#define ViewRenderer_h

#import <GLKit/GLKit.h>

@interface ViewRenderer : NSObject

// TODO: Refactor please
@property float rotAngle;
@property bool isRotating;

- (void)load;
- (void)setup:(GLKView*)view;
- (void)update;
- (void)draw;

@end

#endif /* ViewRenderer_h */
