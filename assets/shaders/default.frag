#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 someColor;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
    FragColor = mix(texture(Texture0, TexCoord), texture(Texture1, TexCoord), 0.5f) * someColor;
};