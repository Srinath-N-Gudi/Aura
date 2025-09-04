#version 330 core 

out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D u_Texture;

void main()
{
	vec3 texColor = vec3(texture(u_Texture, TexCoord));
	FragColor = vec4(texColor, 1.0);
}