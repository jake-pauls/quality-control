#shader vertex
#version 300 es

layout(location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;

void main()
{
    TexCoords = vec3(position.x, position.y, -position.z);
    vec4 pos = _projectionMatrix * _viewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
