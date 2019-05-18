#version 330

struct AmbientLight
{
	vec3 color;
};

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
	
	int hasShadow;
};

struct PointLight
{
	vec3 color;
	vec3 position;

	float cAtt;
	float lAtt;
	float eAtt;
	
	int hasShadow;
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
	
	int hasShadow;
};

smooth in vec2 texCoord;
smooth in vec3 normal;
smooth in vec3 worldPos;

out vec4 outputColor;

uniform sampler2D gSampler;

// data from program
uniform uint ambientLightActive;
uniform AmbientLight ambLight;

uniform uint numDirectionalLights;
uniform DirectionalLight dirLights[4];

uniform uint numPointLights;
uniform PointLight pointLights[4];

uniform uint numSpotLights;
uniform SpotLight spotLights[4];

// calculate light using Phong model
vec3 calculatePhongDirectionalLight( const DirectionalLight light, vec3 normal )
{
	//diffuse
	float diffuse = max( dot( normal, - normalize( light.direction ) ), 0.0);
	
	//specular
	//vec3 viewer = normalize ( vEyeWorldPos - vWorldPos );	
	//vec3 reflected = normalize( reflect( normalize( light.direction ), normal ) );
	//float specular = max( dot( viewer, reflected ), 0 );
	//specular = pow( specular, phongExp ) * specularColor;
	
	//vec3 normalizedLightDir = normalize( light.direction );
	//vec3 halfWay = normalize( viewer + ( - light.direction ) );
	//float specular = max( dot( halfWay, normal ), 0 );
	//specular = pow( specular, phongExp ) * specularColor;
	
	//final result
	vec3 final = light.color * diffuse;
	
	return final;
}

void main()
{
	vec4 surfaceColor = texture(gSampler, texCoord);

	vec3 lightColor = vec3( 0.0f, 0.0f, 0.0f );
	
	// add ambient light contribution
	if( ambientLightActive == uint(1) )
	{
		lightColor.rgb += ambLight.color;
	}
	
	for( uint i = uint(0); i < numDirectionalLights; i++ )
	{
		lightColor += calculatePhongDirectionalLight( dirLights[i], normal );
	}
	
	//for( uint i = uint(0); i < numSpotLights; i++ )
	//{
	//	lightColor += calculatePhongSpotLight( spotLights[i], vNormal );
	//}
	
	outputColor = vec4( surfaceColor.rgb * lightColor, 0.7f );
}


