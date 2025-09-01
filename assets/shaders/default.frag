#version 330 core

// TODO: Custom shader preprocessor to avoid redeclaring structs and uniforms.
struct AmbientLight
{
    vec3 color;
    float intensity;
};
struct DirectionalLight
{
    vec3 color;
    float intensity;
    vec3 direction;
};

in vec4 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;

uniform vec3 viewPosition;

uniform float specularStrength;
uniform int shininess;

// TODO: Inconsistent uniform naming convention.
uniform sampler2D Texture0;

float GetDiffuseDot(vec3 lightDirection, vec3 objectNormal);
float GetSpecularDot(vec3 lightDirection, vec3 objectNormal, vec3 viewDir);

vec4 GetAmbient(AmbientLight light);

void main()
{
    vec3 normal = normalize(Normal);

    vec4 ambient = GetAmbient(ambientLight);
    vec4 diffuse = GetDiffuseDot(directionalLight.direction, normal) * directionalLight.intensity * vec4(directionalLight.color, 1.0);

    // Directional Light Specular
    vec3 directionalLightDir = normalize(directionalLight.direction);
    vec3 viewDir = normalize(viewPosition - FragPos.xyz);
    float specDot = GetSpecularDot(directionalLightDir, normal, viewDir);
    float specular = pow(specDot, shininess) * specularStrength * directionalLight.intensity;
    
    FragColor = texture(Texture0, TexCoord) * (ambient + diffuse + specular);
};

float GetDiffuseDot(vec3 lightDirection, vec3 objectNormal)
{
    return max(dot(objectNormal, lightDirection), 0.0);
}

float GetSpecularDot(vec3 lightDirection, vec3 objectNormal, vec3 viewDir)
{
    vec3 reflectDir = reflect(-lightDirection, objectNormal);
    return max(dot(viewDir, reflectDir), 0.0);
}

vec4 GetAmbient(AmbientLight light)
{
    return vec4(light.color, 1.0) * light.intensity;
}
