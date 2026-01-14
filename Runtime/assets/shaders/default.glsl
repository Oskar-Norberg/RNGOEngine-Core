#vertex_shader
        
#version 330 core

layout (location = POSITION_ATTRIBUTE_POINTER) in vec3 aPos;
layout (location = NORMAL_ATTRIBUTE_POINTER) in vec3 aNormal;
layout (location = TEXCOORD_ATTRIBUTE_POINTER) in vec2 aTexCoord;

// These should not be uniforms.
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    // Outputs
    Normal = mat3(transpose(inverse(Model))) * aNormal;
    TexCoord = aTexCoord;
    FragPos = (Model * vec4(aPos, 1.0)).xyz;
    
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}
        
#fragment_shader

#version 330 core

#include <includes/lights.common>

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPosition;

uniform float specularStrength;
uniform int shininess;

uniform sampler2D ALBEDO_TEXTURE;
uniform sampler2D NORMAL_TEXTURE;
uniform sampler2D SPECULAR_TEXTURE;

void main()
{
    vec3 normal = normalize(Normal);
    LightAccumulation lightAccumulation = GetLight(FragPos, normal, specularStrength, shininess, viewPosition);
    
    FragColor = texture(ALBEDO_TEXTURE, TexCoord) * (lightAccumulation.ambient + lightAccumulation.diffuse)
                    + texture(SPECULAR_TEXTURE, TexCoord) * vec4(vec3(lightAccumulation.specular), 1.0);
};
        