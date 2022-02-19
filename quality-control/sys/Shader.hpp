// quality-control
// Shader.hpp
// 2022-02-18

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <OpenGLES/ES3/gl.h>

class Shader
{
public:
    struct ProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };
    
    enum class Type
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

public:
    static ProgramSource parseShader(const std::string& fileName);
    static GLenum compileShader(GLenum type, const std::string& source);
    static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif /* Shader_hpp */
