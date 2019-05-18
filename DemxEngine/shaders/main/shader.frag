#version 330

struct AmbientLight
{
	vec3 color;
};

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
	
	int shadowIndex;
};

struct PointLight
{
	vec3 color;
	vec3 position;

	float cAtt;
	float lAtt;
	float eAtt;
	
	int shadowIndex;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	
	float coneCosine;
	
	float cAtt;
	float lAtt;
	float eAtt;
	
	int shadowIndex;
};

// shader inputs

smooth in vec2 texCoord;
smooth in vec3 normal;
smooth in vec3 worldPos;
in vec4 shadowCoordDL[4];   // shadows **

// shader outputs
out vec4 outputColor;

// data from program
uniform uint ambientLightActive;
uniform AmbientLight ambLight;

uniform uint numDirectionalLights;
uniform DirectionalLight dirLights[4];

uniform uint numPointLights;
uniform PointLight pointLights[4];

uniform uint numSpotLights;
uniform SpotLight spotLights[4];

// material info
uniform float transparency;
uniform vec3 specularColor;
uniform float phongExp;

uniform vec3 eyeWorldPos;

// texture info
uniform uint numTextures;
uniform sampler2D gSamplers[3];
uniform float textureContr[3];

// shadow map samplers
uniform sampler2DShadow shadowMapSamplerDL[4];	// shadows **

// cube shadow map samplers
uniform samplerCube shadowMapCubeSampler[2];	// shadows **

// calculate light using Phong model
vec3 calculatePhongDirectionalLight( const DirectionalLight light, vec3 normal )
{
    vec3 normalizedLightDir = normalize( light.direction );

	//diffuse
	float diffuse = max( dot( normal, - normalizedLightDir ), 0.0);
	
	//specular
	vec3 viewer = normalize ( eyeWorldPos - worldPos );	
	vec3 halfWay = normalize( viewer + ( - normalizedLightDir ) );
	float specular = max( dot( halfWay, normal ), 0.0 );
	specular = pow( specular, phongExp );
	
	//final result
	vec3 final = light.color * diffuse + light.color * specularColor * specular;
	
	return final;
}

vec3 calculatePhongPointLight( const PointLight light, vec3 normal )
{
	vec3 vecToLightSource = light.position - worldPos;
	vec3 normalizedVecToLightSource = normalize( vecToLightSource );
	float dist = length( vecToLightSource );
	
	//diffuse
	float diffuse = max( dot( normal, normalizedVecToLightSource ), 0 );
	
	//specular
	vec3 viewer = normalize ( eyeWorldPos - worldPos );
	vec3 halfWay = normalize( viewer + normalizedVecToLightSource );
	float specular = max( dot( halfWay, normal ), 0.0 );
	specular = pow( specular, phongExp );
	
	//attenuation
	float attenuation = 1.0 / (light.cAtt + light.lAtt * dist + light.eAtt * dist * dist );
	
	//shadow factor
	float shadowFactor = 1.0f;
	
	if( light.shadowIndex != -1 )
	{
		// sample point light shadow 
		float textureDist = texture( shadowMapCubeSampler[ light.shadowIndex ], -vecToLightSource ).r;
		
		if( dist > ( textureDist + 0.01 ) )
		{
			shadowFactor = 0.0f;
		}
	}

	vec3 final = ( light.color * diffuse + light.color * specularColor * specular ) * attenuation * shadowFactor;
	
	return final;
}

vec3 calculatePhongSpotLight( const SpotLight light, vec3 normal )
{
    vec3 normalizedLightDir = normalize( light.direction );
	
	vec3 vecToLightSource = normalize( light.position - worldPos );
	float dist = length( light.position - worldPos );
	
	float cosine = dot( -vecToLightSource, normalizedLightDir );
	
	if( cosine > light.coneCosine )
	{
		// factor based on how close ray is to cone center
		float factor = (cosine - light.coneCosine) / (1.0f - light.coneCosine);
		
		//diffuse
		float diffuse = max( dot( normal, vecToLightSource ), 0 );
		
		//specular
		vec3 viewer = normalize ( eyeWorldPos - worldPos );
		vec3 halfWay = normalize( viewer + vecToLightSource );
		float specular = max( dot( halfWay, normal ), 0.0 );
		specular = pow( specular, phongExp );
		
		//attenuation
		float spotLightExp = 75.0;
		float spotEffect = max ( dot( - normalizedLightDir, vecToLightSource ), 0 );
		spotEffect = pow( spotEffect, spotLightExp );
		float attenuation = spotEffect / (light.cAtt + light.lAtt * dist + light.eAtt * dist * dist );

		//final
		vec3 final = ( light.color * diffuse + light.color * specularColor * specular ) * factor * attenuation;
		return final;
	}
	else
	{
		return vec3(0.0, 0.0, 0.0);
	}
}

void main()
{
	vec4 surfaceColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	for( uint i = uint(0); i < numTextures; i++ )
	{
		vec4 _texture = ( texture(gSamplers[i], texCoord) * textureContr[i] );
		
		if( _texture.a <= 0.0f )
			discard;
			
		surfaceColor += _texture;
	}
	
	surfaceColor /= numTextures;
	
	float lightHitDL[4] = { 1.0, 1.0, 1.0, 1.0 };
	float lightHitSL[4] = { 1.0, 1.0, 1.0, 1.0 };

	for( uint i = uint(0); i < numDirectionalLights; i++ )
	{
		if( dirLights[i].shadowIndex != -1 )
		{
			int index = dirLights[i].shadowIndex;
			lightHitDL[i] = texture( shadowMapSamplerDL[index], shadowCoordDL[index].xyz );
		}
	}
	
	for( uint i = uint(0); i < numSpotLights; i++ )
	{
		if( spotLights[i].shadowIndex != -1 )
		{
			int index = spotLights[i].shadowIndex;
			lightHitSL[i] = textureProj( shadowMapSamplerDL[index], shadowCoordDL[index].xyzw );
		}
	}
	
	vec3 lightColor = vec3( 0.0f, 0.0f, 0.0f );
	
	// add ambient light contribution
	if( ambientLightActive == uint(1) )
	{
		lightColor += ambLight.color;
	}
	
	vec3 normalized = normalize( normal );
	
	for( uint i = uint(0); i < numDirectionalLights; i++ )
	{
		lightColor += calculatePhongDirectionalLight( dirLights[i], normalized ) * lightHitDL[i];		
	}
	
	for( uint i = uint(0); i < numPointLights; i++ )
	{
		lightColor += calculatePhongPointLight( pointLights[i], normalized );
	}
	
	for( uint i = uint(0); i < numSpotLights; i++ )
	{
		lightColor += calculatePhongSpotLight( spotLights[i], normalized ) * lightHitSL[i];
	}
	
	outputColor = vec4( lightColor * surfaceColor.rgb, transparency );
}
