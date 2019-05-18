#version 330

// data from program
uniform mat4 projectionViewModelMatrix;

// shader input
layout (location = 0) in vec3 inPosition;

// shader output
out vec3 texCoord;

void main()
{
	gl_Position = 	projectionViewModelMatrix*
					vec4(inPosition, 1.0);
					
	texCoord = inPosition;
}