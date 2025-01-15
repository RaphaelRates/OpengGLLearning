
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;

void main()
{
    float waveIntensity = 0.005;
    float speed = 3.0;          

    float offsetX = sin(TexCoords.y * 10.0 + time * speed) * waveIntensity;
    float offsetY = cos(TexCoords.x * 10.0 + time * speed) * waveIntensity;

    // Aplica a distorção nas coordenadas de textura
    vec2 distortedTexCoords = TexCoords + vec2(offsetX, offsetY);

    // Amostra a cor da textura no framebuffer
    vec3 color = texture(screenTexture, distortedTexCoords).rgb;

    // Aplica um efeito de tonalidade para um visual mais interessante
    vec3 tint = vec3(0.8, 0.9, 1.2); // Leve tonalidade azulada
    color *= tint;

    FragColor = vec4(color, 1.0); // Cor final do pixel
} 