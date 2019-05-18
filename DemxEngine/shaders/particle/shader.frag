#version 330

// shader inputs
smooth in vec2 texCoord;
smooth in vec4 instanceColor;

// shader outputs
out vec4 outputColor;

uniform uint numTextures;
uniform sampler2D gSamplers[3];
uniform float textureContr[3];

void main()
{
	outputColor = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	vec4 textureColor = vec4( 0.0, 0.0, 0.0, 0.0 );
	for( uint i = uint(0); i < numTextures; i++ )
	{
		vec4 _texture = ( texture(gSamplers[i], texCoord) * textureContr[i] );
		textureColor += _texture;
	}
	
	textureColor /= numTextures;
	
	outputColor = vec4( instanceColor.rgb * textureColor.rgb, instanceColor.a );
}


