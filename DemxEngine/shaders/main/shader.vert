#version 330

// data from program
uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform mat4 depthBiasMVPDL[4];	//shadows **

// shader input
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

// shader output
smooth out vec2 texCoord;
smooth out vec3 normal;
smooth out vec3 worldPos;

out vec4 shadowCoordDL[4];	// shadows **

void main()
{
	gl_Position = 	projectionViewMatrix*
					modelMatrix*
					vec4(inPosition, 1.0);
					
	texCoord = inCoord;
	
	for( int i = 0; i < 4; i++ )
	{
		shadowCoordDL[i] = depthBiasMVPDL[i] * vec4( inPosition, 1.0 );
		
		// apply bias to avoid artifacts
		shadowCoordDL[i].z -= 0.0001;
	}
	
	// calculate normal of vertex based on normal matrix
	normal = normalize ( (normalMatrix * vec4( inNormal, 0.0 )).xyz );
	
	//vec4 worldPosVertex = modelMatrix * vec4( inPosition, 1.0 );
	//worldPos = worldPosVertex.xyz;
	
	// calculate world position of vertex
	worldPos = (modelMatrix * vec4( inPosition, 1.0 )).xyz;
}