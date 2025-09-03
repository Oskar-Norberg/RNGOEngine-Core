#version 330 core

#include <includes/lights.common>

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPosition;

uniform float specularStrength;
uniform int shininess;

// TODO: Inconsistent uniform naming convention.
uniform sampler2D Texture0;

void main()
{
    vec3 normal = normalize(Normal);

    LightAccumulation lightAccumulation = GetLight(FragPos, normal, specularStrength, shininess, viewPosition);
    
    FragColor = texture(Texture0, TexCoord) * (lightAccumulation.ambient + lightAccumulation.diffuse + vec4(vec3(lightAccumulation.specular), 1.0));
};

