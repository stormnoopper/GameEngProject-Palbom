#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool isBomb;  // Flag to indicate if this is a bomb

void main()
{    
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // If this is a bomb, force black color
    if (isBomb)
    {
        texColor = vec4(0.1, 0.1, 0.1, 1.0); // Black color for bomb
    }
    // For characters, use original texture color (with fallback for very dark textures)
    else if (texColor.r < 0.01 && texColor.g < 0.01 && texColor.b < 0.01)
    {
        texColor = vec4(0.4, 0.7, 0.9, 1.0); // Default color for characters
    }
    
    // Apply lighting
    float ambientStrength = isBomb ? 0.2 : 0.4;  // Darker for bombs
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting
    float specularStrength = isBomb ? 0.1 : 0.3;  // Less specular for bombs
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine lighting with texture
    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    
    // For bombs, limit maximum brightness to keep it dark
    if (isBomb)
    {
        result = min(result, vec3(0.3, 0.3, 0.3)); // Maximum dark gray/black
    }
    
    FragColor = vec4(result, texColor.a);
}







