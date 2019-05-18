#version 330

// data from program
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// shader input
layout (location = 0) in vec3 inPosition;

void main()
{
	gl_Position = 	projectionMatrix*
					viewMatrix*
					modelMatrix*
					vec4(inPosition, 1.0);
}