// quality-control
// Assert.hpp
// 2022-02-18

#ifndef Assert_hpp
#define Assert_hpp

#include <iostream>
#include <stdio.h>
#include <OpenGLES/ES3/gl.h>

#define __SHORT_NAME__ strrchr("/" __FILE__, '/') + 1

#define LOG(x) \
    std::cout << "[QualityControl::" << __SHORT_NAME__ << "(" << __LINE__ << ")]" << x << std::endl;

#define ASSERT(x) if (!(x)) {}

#define GL_CALL(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int lineNumber);

#endif /* Assert_hpp */
