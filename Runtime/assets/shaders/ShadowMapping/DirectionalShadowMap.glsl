#vertex_shader
        
#version 330 core

layout (location = POSITION_ATTRIBUTE_POINTER) in vec3 aPos;

uniform mat4 LightSpaceMatrix;
uniform mat4 Model;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    gl_Position = LightSpaceMatrix * Model * vec4(aPos, 1.0);
}
        
#fragment_shader

#version 330 core

void main()
{
    // Do nothing
}
        