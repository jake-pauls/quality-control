// quality-control
// Assert.hpp
// 2022-02-18

#ifndef Assert_hpp
#define Assert_hpp

#include <iostream>
#include <OpenGLES/ES3/gl.h>

#define LOG(x) \
    std::cout << "[QualityControl::" __FILE__ << "(" << __LINE__ << ")]" << x << std::endl;

#define ASSERT(x) if (!(x)) {}

#define GL_CALL(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Assert_hpp */
