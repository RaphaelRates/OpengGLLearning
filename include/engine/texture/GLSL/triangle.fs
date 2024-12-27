#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.34);
}