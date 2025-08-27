#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec4 someColor;
uniform sampler2D Texture0;

void main()
{
    FragColor = texture(Texture0, TexCoord);
};