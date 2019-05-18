#version 330

in vec3 worldPos;

uniform vec3 lightPos;
uniform sampler2D gSamplers;

out float color;

void main()
{
	color = length( worldPos - lightPos );
}
