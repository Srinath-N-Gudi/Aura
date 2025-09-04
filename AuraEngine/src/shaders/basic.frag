#version 330 core 

out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D u_Texture;
uniform int useTexture;
uniform vec3 albedoColor;

void main()
{
	vec3 pixelColor;
	if (useTexture == 1) // True
		pixelColor = vec3(texture(u_Texture, TexCoord));
	else
		pixelColor = albedoColor; 
	FragColor = vec4(pixelColor, 1.0);
}