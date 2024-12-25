#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;  // Adicionando o uniforme para a segunda textura

void main()
{
    // Mesclar as duas texturas com a função mix
    FragColor = mix(texture(ourTexture2, vec2(TexCoord.x * 2, TexCoord.y * 2)),  texture(ourTexture,  TexCoord), 0.5); 
}
