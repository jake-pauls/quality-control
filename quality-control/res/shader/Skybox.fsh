#shader fragment
#version 300 es

precision highp float;

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube _skyboxSampler;

void main()
{
    FragColor = texture(_skyboxSampler, TexCoords);
}
