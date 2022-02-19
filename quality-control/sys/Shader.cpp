// quality-control
// Shader.cpp
// 2022-02-18

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Shader.hpp"
#include "Assert.hpp"

/**
 * Parses shader with a header format -> #shader TYPE
 * Allows to combine vertex/fragment shaders in a single file if small and/or tightly coupled
 */
Shader::ProgramSource Shader::parseShader(const std::string& fileName)
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
GLenum Shader::compileShader(GLenum type, const std::string& source)
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
GLuint Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint glProgram = glCreateProgram();
    GLenum vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLenum fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    // Attach, Link, and Validate shader program
    GL_CALL(glAttachShader(glProgram, vs));
    GL_CALL(glAttachShader(glProgram, fs));
    GL_CALL(glLinkProgram(glProgram));
    GL_CALL(glValidateProgram(glProgram));
    
    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));
    
    return glProgram;
}
