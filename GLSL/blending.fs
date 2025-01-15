
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{             
    if (gl_FragCoord.x < 300)
        FragColor = texture(texture1, TexCoords) * vec4(1.0,0.2,1.0,1.0);
    else 
        FragColor = texture(texture1, TexCoords);
}