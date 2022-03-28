#shader fragment
#version 300 es

precision highp float;

struct Light {
    vec3 position;
    vec3 direction;
};

struct Model {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
};

out vec4 o_fragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec4 FragPos;

uniform sampler2D texSampler;

uniform vec3 _viewPosition;
uniform float _shininess;

uniform Light _light;
uniform Model _model;

void main()
{
    // Ambient
    vec4 ambient = _model.ambient  * texture(texSampler, TexCoords).rgba;
    
    // Diffuse
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(_light.position - vec3(FragPos));
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = _model.diffuse * diff;
    
    // Specular
    vec3 viewDir = normalize(_viewPosition - vec3(FragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), _shininess);
    vec4 specular = _model.specular * spec;
    
    if (spec < 0.0f) {
        specular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    o_fragColor = ambient + diffuse + specular;
}
