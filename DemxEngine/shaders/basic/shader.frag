#version 330

// shader inputs
//smooth in vec2 texCoord;

// shader outputs
out vec4 outputColor;

// shader uniforms
//uniform sampler2D gSampler;
uniform vec4 color;

void main()
{
	//vec4 vTexColor = texture2D( gSampler, texCoord );
	outputColor = color;
}


