// quality-control
// TextureLoader.mm
// 2022-03-27

#import "ViewRenderer.h"
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
    
    GLubyte* spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    CGContextRelease(spriteContext);
    
    GLuint textureId;
    
    GL_CALL(glGenTextures(1, &textureId));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData));
    
    free(spriteData);
    
    return textureId;
}

+ (GLuint)CreateSkyboxCubeMap
{
    NSArray* fileNames = [NSArray arrayWithObjects: @"Skybox_Middle.jpg", @"Skybox_Middle.jpg", @"Skybox_Top.jpg", @"Skybox_Bottom.jpg", @"Skybox_Middle.jpg", @"Skybox_Middle.jpg", nil];
    
    GLuint cubeMapeTextureId;
    
    GL_CALL(glGenTextures(1, &cubeMapeTextureId));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapeTextureId));
    
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    
    // Prevent cube map from seaming
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    
    for (int i = 0; i < [fileNames count]; i++)
    {
        const char* fileName = [[fileNames objectAtIndex:i] UTF8String];
        NSString* nsFileName = [NSString stringWithUTF8String:[ViewRenderer RetrieveFilePathByName:fileName]];
        
        [TextureLoader ParseCubeMapImages:nsFileName :i];
    }
    
    return cubeMapeTextureId;
}

+ (void)ParseCubeMapImages:(NSString *)fileName:(unsigned int)i
{
    CGImageRef spriteImage = [UIImage imageNamed:fileName].CGImage;
    ASSERT(spriteImage);
    
    size_t width = CGImageGetWidth(spriteImage);
    size_t height = CGImageGetHeight(spriteImage);
    
    GLubyte* spriteData = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width*4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
    CGContextRelease(spriteContext);
    
    GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData));
    
    free(spriteData);
}

/**
 * "Trampoline" C function called by C++ to retrieve texture data
 */
unsigned int RetrieveObjectiveCTextureData(const char* fileName)
{
    return [TextureLoader LoadTextureFile: fileName];
}

unsigned int RetrieveObjectiveCSkyboxData()
{
    return [TextureLoader CreateSkyboxCubeMap];
}

@end
