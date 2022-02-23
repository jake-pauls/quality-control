// quality-control
// Shader.hpp
// 2022-02-18

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <unordered_map>
#include <OpenGLES/ES3/gl.h>

class Shader
{
private:
    unsigned int _rendererID;
    std::string _filePaths[2];
    std::unordered_map<std::string, int> _uniformLocations;
    
public:
    struct ProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };
    
    enum Type
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    Shader();
    Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
    
    void Bind();
    void Unbind();
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUniformMatrix4fv(const std::string& name, const GLfloat* matrix);
    
private:
    ProgramSource ParseShader(const std::string& fileName);
    GLenum CompileShader(GLenum type, const std::string& source);
    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};

#endif /* Shader_hpp */
