#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;

void main()
{
    // // Constante para o deslocamento básico do kernel
    // const float offset = 1.0 / 300.0;

    // // Coordenadas ajustadas com base em ondas animadas
    // vec2 waveOffset = vec2(sin(time + TexCoords.y * 10.0) * 0.01, 
    //                        cos(time + TexCoords.x * 10.0) * 0.01);

    // // Vetores de deslocamento do kernel
    // vec2 offsets[9] = vec2[](
    //     vec2(-offset,  offset) + waveOffset, // top-left
    //     vec2( 0.0f,    offset) + waveOffset, // top-center
    //     vec2( offset,  offset) + waveOffset, // top-right
    //     vec2(-offset,  0.0f)   + waveOffset, // center-left
    //     vec2( 0.0f,    0.0f)   + waveOffset, // center-center
    //     vec2( offset,  0.0f)   + waveOffset, // center-right
    //     vec2(-offset, -offset) + waveOffset, // bottom-left
    //     vec2( 0.0f,   -offset) + waveOffset, // bottom-center
    //     vec2( offset, -offset) + waveOffset  // bottom-right
    // );

    // // Kernel Gaussiano adaptado para variações com tempo
    // float kernel[9] = float[](
    //     1.0 / 6.0, 2.0 / 6.0, 1.0 / 6.0,
    //     2.0 / 6.0, 4.0 / 6.0, 2.0 / 6.0,
    //     1.0 / 6.0, 2.0 / 6.0, 1.0 / 6.0
    // );

    // // Amostragem da textura usando os offsets com distorção
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++)
    // {
    //     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    // }

    // // Aplicação do kernel para cálculo da cor final
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];

    // // Ajustar cor para simular reflexos na água
    // col *= vec3(0.1, 0.3, 0.8) + 0.2 * abs(sin(time /3));

    // FragColor = vec4(col, 1.0);

    // Coordenadas ajustadas para o centro da tela
    // vec2 uv = TexCoords * 2.0 - 1.0;
    // float radius = length(uv); // Distância do centro

    // // Adicionar uma rotação baseada no tempo para criar o efeito de redemoinho
    // float angle = atan(uv.y, uv.x);
    // float swirl = 0.5 * sin(time) * radius; // Intensidade do redemoinho
    // angle += swirl;

    // // Converter de volta para coordenadas cartesianas
    // vec2 distorted = vec2(cos(angle), sin(angle)) * radius;

    // // Ajustar de volta para o intervalo 0-1
    // distorted = (distorted + 1.0) * 0.5;

    // // Amostrar a textura distorcida
    // vec3 color = texture(screenTexture, distorted).rgb;

    // // Adicionar um efeito de fade-out nas bordas da tela
    // float vignette = smoothstep(0.8, 0.4, radius); // Suaviza as bordas
    // color *= vignette;

    // FragColor = vec4(color, 1.0);

    vec2 uv = TexCoords * 2.0 - 1.0;
    float radius = length(uv); // Distância do centro

    // Criar uma distorção em onda baseada no raio
    float wave = sin(10.0 * radius - time * 5.0) * 0.03;

    // Aplicar o deslocamento radial
    vec2 distorted = uv + uv * wave;

    // Ajustar de volta para o intervalo 0-1
    distorted = (distorted + 1.0) * 0.5;

    // Amostrar a textura distorcida
    vec3 color = texture(screenTexture, distorted).rgb;

    // Adicionar uma modulação de brilho para intensificar o efeito
    color *= 0.8 + 0.2 * sin(time * 10.0);

    FragColor = vec4(color, 1.0);


}
