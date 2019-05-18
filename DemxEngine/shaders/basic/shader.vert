#version 330

// data from program
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// shader input
layout (location = 0) in vec3 inPosition;
//layout (location = 1) in vec2 inCoord;
//layout (location = 2) in vec3 inNormal;

// shader output
//smooth out vec2 texCoord;

void main()
{
	gl_Position = 	projectionMatrix*
					viewMatrix*
					modelMatrix*
					vec4(inPosition, 1.0);
	//texCoord = inCoord;
}