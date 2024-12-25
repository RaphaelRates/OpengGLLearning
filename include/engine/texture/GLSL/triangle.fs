#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    vec4 texColor1 = texture(ourTexture, TexCoord); // Cor da textura base
    vec4 texColor2 = texture(ourTexture2, TexCoord * 2.0); // Cor da segunda textura, com coordenadas escaladas

    // Mesclar as duas texturas com a função mix
    FragColor = mix(texColor1, texColor2, 0.5); // Mesclagem 50/50
}
