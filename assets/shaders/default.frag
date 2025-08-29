#version 330 core

// TODO: This should be in a common include file somehow. Custom shader preprocessor?
struct AmbientLight 
{
    vec3 color;
    float intensity;
};
uniform AmbientLight ambientLight;

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 someColor;

uniform sampler2D Texture0;

void main() 
{
    FragColor = texture(Texture0, TexCoord) * ambientLight.intensity * vec4(ambientLight.color, 1.0);
};