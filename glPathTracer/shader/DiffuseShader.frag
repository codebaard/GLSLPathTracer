#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DiffuseMap;

void main()
{    
    FragColor = texture(DiffuseMap, TexCoords);
}