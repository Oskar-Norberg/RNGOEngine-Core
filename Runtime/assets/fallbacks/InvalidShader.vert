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