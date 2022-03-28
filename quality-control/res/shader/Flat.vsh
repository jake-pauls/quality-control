#shader vertex
#version 300 es

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoordIn;

out vec2 TexCoords;
out vec3 Normals;
out vec4 FragPos;

uniform mat4 _mvpMatrix;

void main()
{
    TexCoords = TexCoordIn;
    
    Normals = Normal;
    
    FragPos = _mvpMatrix * Position;
    
    gl_Position = _mvpMatrix * Position;
}
