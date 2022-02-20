// quality-control
// Shader.cpp
// 2022-02-18

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.hpp"
#include "Assert.hpp"

Shader::Shader()
{ }

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) : _filePaths{vertexFilePath, fragmentFilePath}, _rendererID(0)
{
    ProgramSource source {
        ParseShader(vertexFilePath).vertexSource,
        ParseShader(fragmentFilePath).fragmentSource
    };
    
    _rendererID = CreateShader(source.vertexSource, source.fragmentSource);
    
    LOG("Shader program object created for shader with ID #" << _rendererID);
    
    GL_CALL(glUseProgram(_rendererID));
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(_rendererID));
}

void Shader::Bind()
{
    GL_CALL(glUseProgram(_rendererID));
}

void Shader::Unbind()
{
    GL_CALL(glUseProgram(0));
}

/**
 * Retrieves the program ID for a particular uniform on a Shader
 * Used locally for an easier API in setting uniform values
 */
int Shader::GetUniformLocation(const std::string& name)
{
    // Return uniform location if it exists in the cache
    if (_uniformLocations.find(name) != _uniformLocations.end())
        return _uniformLocations[name];
    
    // Check if uniform exists and add it to cache
    LOG("Searching for " << name << " on Shader with ID " << _rendererID);
    GL_CALL(int location = glGetUniformLocation(_rendererID, name.c_str()));
    
    if (location == -1)
        LOG("WARN: Uniform \"" << name << "\" not found on shader with ID " << _rendererID);
    
    _uniformLocations[name] = location;

    return location;
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const GLfloat* matrix)
{
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

/**
 * Parses shader with a header format -> #shader TYPE
 * Allows to combine vertex/fragment shaders in a single file if small and/or tightly coupled
 */
Shader::ProgramSource Shader::ParseShader(const std::string& fileName)
{
    std::ifstream stream(fileName);
    
    std::string line;
    std::stringstream ss[2];
    Shader::Type type = Shader::Type::NONE;
    
    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = Shader::Type::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = Shader::Type::FRAGMENT;
            }
        }
        else
        {
            ss[(int) type] << line << "\n";
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

/**
 * Compiles shaders from a string with shader contents
 */
GLenum Shader::CompileShader(GLenum type, const std::string& source)
{
    GLenum shaderId = glCreateShader(type);
    const char* src = source.c_str();
    
    GL_CALL(glShaderSource(shaderId, 1, &src, nullptr));
    GL_CALL(glCompileShader(shaderId));
    
    int ivResult;
    GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult));
    
    if (ivResult == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
        
        char* message = (char*) malloc(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shaderId, length, &length, message));
        
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        
        GL_CALL(glDeleteShader(shaderId));
        
        return 0;
    }
    
    return shaderId;
}

/**
 * Compiles, creates, and attaches shader programs
 * Discards/deletes shaders from state after creation
 */
GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint glProgram = glCreateProgram();
    GLenum vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLenum fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    // Attach, Link, and Validate shader program
    GL_CALL(glAttachShader(glProgram, vs));
    GL_CALL(glAttachShader(glProgram, fs));
    GL_CALL(glLinkProgram(glProgram));
    GL_CALL(glValidateProgram(glProgram));
    
    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));
    
    return glProgram;
}
