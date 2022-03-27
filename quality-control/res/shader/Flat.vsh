#shader vertex
#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordIn;

out vec2 texCoordOut;

uniform mat4 _mvpMatrix;

void main()
{
    texCoordOut = texCoordIn;
    
    gl_Position = _mvpMatrix * position;
}
