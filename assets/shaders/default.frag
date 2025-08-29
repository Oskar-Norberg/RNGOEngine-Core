#version 330 core

// TODO: Custom shader preprocessor to avoid redeclaring structs and uniforms.
struct AmbientLight
{
    vec3 color;
    float intensity;
};
uniform AmbientLight ambientLight;

struct DirectionalLight{
    vec3 color;
    float intensity;
    vec3 direction;
};
uniform DirectionalLight directionalLight;

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec4 someColor;
uniform sampler2D Texture0;

void main() 
{
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -directionalLight.direction), 0.0);
    vec4 diffuse = vec4(directionalLight.color, 1.0) * directionalLight.intensity * diff;
    
    vec4 ambient = vec4(ambientLight.color, 1.0) * ambientLight.intensity;
    FragColor = texture(Texture0, TexCoord) * (diffuse + ambient);
};