#version 330

// Ouput data
layout(location = 0) out float fragmentdepth;

smooth in vec2 texCoord;

uniform sampler2D gSamplers;

void main()
{
	vec4 _texture = texture( gSamplers, texCoord );
	
	if( _texture.a <= 0.0f )
		discard;
		
	fragmentdepth = gl_FragCoord.z;
}
