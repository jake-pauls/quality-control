// quality-control
// TextureLoader.h
// 2022-03-27

#ifndef TextureLoader_h
#define TextureLoader_h

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

@interface TextureLoader : NSObject

+ (GLuint)LoadTextureFile:(const char *)fileName;

@end

#endif /* TextureLoader_h */
