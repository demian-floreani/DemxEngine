#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 depthMVP;
uniform mat4 model;

out vec3 worldPos;

void main()
{
	vec4 pos = vec4( inPosition, 1.0 );
	gl_Position =  depthMVP * pos;
	worldPos = (model * pos).xyz;
}
