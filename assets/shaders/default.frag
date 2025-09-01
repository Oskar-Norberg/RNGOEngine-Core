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
struct PointLight
{
    vec3 color;
    float intensity;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
// TODO: Ugly hardcoded limit.
uniform PointLight pointLights[16];
uniform int numPointLights;

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

    // Ambient Light
    vec4 ambient = GetAmbient(ambientLight);
    
    // Directional Light
    vec4 directionalDiffuse = GetDiffuseDot(directionalLight.direction, normal) * directionalLight.intensity * vec4(directionalLight.color, 1.0);
    // Directional Light Specular
    vec3 directionalLightDir = normalize(directionalLight.direction);
    vec3 directionalLightViewDir = normalize(viewPosition - FragPos);
    float directionalSpecDot = GetSpecularDot(directionalLightDir, normal, directionalLightViewDir);
    float directionalSpecular = pow(directionalSpecDot, shininess) * specularStrength * directionalLight.intensity;
    
    // Point Lights
    vec4 pointLightDiffuse = vec4(0.0);
    float pointLightSpecular = 0.0;
    for (int i = 0; i < numPointLights; i++) 
    {
        float distance = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));
        
        vec3 pointLightDir = normalize(pointLights[i].position - FragPos);
        pointLightDiffuse += GetDiffuseDot(pointLightDir, normal) * pointLights[i].intensity * vec4(pointLights[i].color, 1.0) * attenuation;
        
        // Specular
        vec3 pointLightViewDir = normalize(viewPosition - FragPos);
        float pointLightSpecDot = GetSpecularDot(pointLightDir, normal, pointLightViewDir);
        pointLightSpecular += pow(pointLightSpecDot, shininess) * specularStrength * pointLights[i].intensity * attenuation;
    }

    vec4 diffuse = directionalDiffuse + pointLightDiffuse;
    float specular = directionalSpecular + pointLightSpecular;
    
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
