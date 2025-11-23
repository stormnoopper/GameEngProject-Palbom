#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (texColor.r < 0.01 && texColor.g < 0.01 && texColor.b < 0.01)
    {
        FragColor = vec4(0.4, 0.7, 0.9, 1.0);
    }
    else
    {
        FragColor = texColor;
    }
}







