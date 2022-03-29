// quality-control
// TextureLoader.mm
// 2022-03-27

#import "TextureLoader.h"

#include "Assert.hpp"

@implementation TextureLoader

+ (GLuint)LoadTextureFile:(const char *)fileName
{
    NSString* cFileName = [NSString stringWithUTF8String:fileName];
    
    CGImageRef spriteImage = [UIImage imageNamed:cFileName].CGImage;
    ASSERT(spriteImage);
    
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte *spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    CGContextRelease(spriteContext);
    
    GLuint texId;
    
    GL_CALL(glGenTextures(1, &texId));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texId));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData));
    free(spriteData);
    
    return texId;
}

/**
 * "Trampoline" C function called by C++ to retrieve texture data
 */
unsigned int RetrieveObjectiveCTextureData(const char* fileName)
{
    return [TextureLoader LoadTextureFile: fileName];
}

@end
