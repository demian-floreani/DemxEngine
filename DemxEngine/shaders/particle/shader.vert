#version 330

// data from program
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

// shader input
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
// instancing
layout (location = 2) in vec4 inInstancePosition;
layout (location = 3) in vec4 inInstanceColor;

// shader output
smooth out vec2 texCoord;
smooth out vec4 instanceColor;

void main()
{
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix[3][0] = inInstancePosition[0];
	modelMatrix[3][1] = inInstancePosition[1];
	modelMatrix[3][2] = inInstancePosition[2];
	
	mat4 MV = viewMatrix * modelMatrix;
	mat4 billboardMatrix = mat4(1.0f);
	billboardMatrix[3][0] = MV[3][0];
	billboardMatrix[3][1] = MV[3][1];
	billboardMatrix[3][2] = MV[3][2];
	
	gl_Position = 	projectionMatrix*
					billboardMatrix*
					vec4(inPosition * inInstancePosition[3], 1.0);
	
	texCoord = inCoord;
	instanceColor = inInstanceColor;
}

