#version 330

// shader inputs
in vec3 texCoord;

// shader outputs
out vec4 outputColor;

// shader uniforms
uniform samplerCube gSampler;
uniform vec4 color;

void main()
{
	vec4 surfaceColor = texture( gSampler, texCoord );
	outputColor = surfaceColor * color;
}


