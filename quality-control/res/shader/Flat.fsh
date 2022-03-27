#shader fragment
#version 300 es

precision highp float;

out vec4 o_fragColor;

in vec2 texCoordOut;

uniform sampler2D texture_diffuse1;

void main()
{
    o_fragColor = texture(texture_diffuse1, texCoordOut);
}
