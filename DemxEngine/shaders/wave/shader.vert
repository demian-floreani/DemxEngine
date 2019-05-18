#version 330

// data from program
uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform mat4 depthBiasMVPDL[4];	//shadows **

uniform float wave;
uniform float modifier;
uniform float divisor;

// shader input
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

// shader output
smooth out vec2 texCoord;
smooth out vec3 normal;
smooth out vec3 worldPos;

out vec4 shadowCoordDL[4];	// shadows **

void main()
{
	vec3 coord = inPosition;
	float displacement = wave + ( coord.x / divisor ) * 360.0;
	coord.y = modifier * sin( radians( displacement ) );
	gl_Position = 	projectionViewMatrix*
					modelMatrix*
					vec4(coord, 1.0);
					
	texCoord = inCoord;
	
	for( int i = 0; i < 4; i++ )
	{
		shadowCoordDL[i] = depthBiasMVPDL[i] * vec4( inPosition, 1.0 );
		
		// apply bias to avoid artifacts
		shadowCoordDL[i].z -= 0.0001;
	}
	
	// calculate normal of vertex based on normal matrix
	vec4 normalVec = normalMatrix * vec4( - vec3(modifier * cos( radians( displacement ) ), -1.0, 0.0), 0.0 );
	normal = normalize( normalVec.xyz );	
	
	// calculate world position of vertex
	worldPos = (modelMatrix * vec4( coord, 1.0 )).xyz;
}
