#version 330 core

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    // TODO: Would be nice to add the "missing texture" here.
    // For now, just output solid red.
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
};
