#vertex_shader

#version 330 core

layout (location = POSITION_ATTRIBUTE_POINTER) in vec2 aPos;
layout (location = TEXCOORD_ATTRIBUTE_POINTER) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    // Outputs
    TexCoord = aTexCoord;
    
    gl_Position = vec4(aPos, 0.0f, 1.0);
}
        
#fragment_shader
        
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ScreenTexture;

void main()
{
    FragColor = texture(ScreenTexture, TexCoord);
}
        