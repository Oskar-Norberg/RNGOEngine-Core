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
struct Spotlight{
    vec3 color;
    float intensity;
    vec3 position;
    float cutoff;
    vec3 direction;
    float outerCutoff;
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
uniform PointLight pointLights[NR_OF_SPOTLIGHTS];
uniform int numPointLights;

uniform Spotlight spotlights[NR_OF_SPOTLIGHTS];
uniform int numSpotlights;

uniform vec3 viewPosition;

uniform float specularStrength;
uniform int shininess;

// TODO: Inconsistent uniform naming convention.
uniform sampler2D Texture0;

float GetDiffuseDot(vec3 lightDirection, vec3 objectNormal);
float GetSpecularDot(vec3 lightDirection, vec3 objectNormal, vec3 viewDir);

vec4 GetAmbient(AmbientLight light);

float GetAttenuation(float distance, float constant, float linear, float quadratic);

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
        float attenuation = GetAttenuation(distance, pointLights[i].constant, pointLights[i].linear, pointLights[i].quadratic);
        
        vec3 pointLightDir = normalize(pointLights[i].position - FragPos);
        pointLightDiffuse += max(GetDiffuseDot(pointLightDir, normal) * pointLights[i].intensity * vec4(pointLights[i].color, 1.0) * attenuation, 0.0);
        
        // Specular
        vec3 pointLightViewDir = normalize(viewPosition - FragPos);
        float pointLightSpecDot = GetSpecularDot(pointLightDir, normal, pointLightViewDir);
        pointLightSpecular += max(pow(pointLightSpecDot, shininess) * specularStrength * pointLights[i].intensity * attenuation, 0.0);
    }
    
    // Spotlights
    vec4 spotlightDiffuse = vec4(0.0);
    float spotlightSpecular = 0.0;
    for (int i = 0; i < numSpotlights; i++)
    {
        vec3 spotlightDir = normalize(spotlights[i].position - FragPos);

        float theta = dot(spotlightDir, normalize(-spotlights[i].direction));
        float epsilon = spotlights[i].cutoff - spotlights[i].outerCutoff;
        float intensity = clamp((theta - spotlights[i].outerCutoff) / epsilon, 0.0, 1.0);

        float distance = length(spotlights[i].position - FragPos);
        float attenuation = GetAttenuation(distance, spotlights[i].constant, spotlights[i].linear, spotlights[i].quadratic);

        // Diffuse
        spotlightDiffuse += max(GetDiffuseDot(spotlightDir, normal) * attenuation * intensity * spotlights[i].intensity * vec4(spotlights[i].color, 1.0), 0.0);

        // Specular
        vec3 spotlightViewDir = normalize(viewPosition - FragPos);
        float spotlightSpecDot = GetSpecularDot(spotlightDir, normal, spotlightViewDir);
        spotlightSpecular += max(pow(spotlightSpecDot, shininess) * specularStrength * spotlights[i].intensity * attenuation * intensity, 0.0);
    }

    vec4 diffuse = directionalDiffuse + pointLightDiffuse + spotlightDiffuse;
    float specular = directionalSpecular + pointLightSpecular + spotlightSpecular;
    
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

float GetAttenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}