#vertex_shader
        
#version 330 core

layout (location = POSITION_ATTRIBUTE_POINTER) in vec3 aPos;
layout (location = NORMAL_ATTRIBUTE_POINTER) in vec3 aNormal;
layout (location = TEXCOORD_ATTRIBUTE_POINTER) in vec2 aTexCoord;

// These should not be uniforms.
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

// Shadow Mapping
uniform mat4 LightSpaceMatrix;

// TODO: Out block
out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec4 FragPosLightSpace;

void main()
{
    // Outputs
    Normal = mat3(transpose(inverse(Model))) * aNormal;
    TexCoord = aTexCoord;
    FragPos = (Model * vec4(aPos, 1.0)).xyz;
    FragPosLightSpace = LightSpaceMatrix * vec4(FragPos, 1.0);
    
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}
        
#fragment_shader

#version 330 core

#include <includes/lights.common>

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform vec3 viewPosition;

uniform float specularStrength;
uniform int shininess;

uniform sampler2D ALBEDO_TEXTURE;
uniform sampler2D NORMAL_TEXTURE;
uniform sampler2D SPECULAR_TEXTURE;

uniform sampler2D DIRECTIONAL_SHADOWMAP_TEXTURE;

#include <includes/shadowmapping.common>

void main()
{
    vec3 normal = normalize(Normal);
    LightAccumulation lightAccumulation = GetLight(FragPos, normal, specularStrength, shininess, viewPosition);
    float shadow = CalculateShadows(normal, directionalLight.direction, FragPosLightSpace);

    vec4 albedoTexture = texture(ALBEDO_TEXTURE, TexCoord);
    vec4 specularTexture = texture(SPECULAR_TEXTURE, TexCoord);

    vec3 lighting =
    lightAccumulation.ambient.rgb * albedoTexture.rgb +
    (1.0 - shadow) * (
        lightAccumulation.diffuse.rgb * albedoTexture.rgb +
        lightAccumulation.specular * specularTexture.rgb
    );

    FragColor = vec4(lighting, albedoTexture.a);
};
        