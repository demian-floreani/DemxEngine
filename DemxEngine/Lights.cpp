#include "stdafx.h"
#include "Lights.h"

#include "OrthoShadow.h"
#include "ProjectionShadow.h"
#include "ProjectionCubeShadow.h"

/** 
*	@brief Constructor for the base class for lights
* 
*	@author Demian Floreani
*
*	@param[in] color COLOR3 representing the light's color
*
*	@return None
*/
Demx::Light::Light( COLOR3 color )
{
	this->color = color;
	this->active = GL_TRUE;
	this->shadow = nullptr;
}

/** 
*	@brief Destructor for light base class
* 
*	@author Demian Floreani
*
*	@return None
*/
Demx::Light::~Light()
{
	if( shadow != nullptr )
		delete shadow;
}

GLvoid Demx::Light::setName( std::string name )
{
	this->name = name;
}

/** 
*	@brief Toggle the light from active to inactive and viceversa
* 
*	@author Demian Floreani
*
*	@return None
*/
void Demx::Light::toggle()
{
	active = !active;
}

void Demx::Light::castShadow( GLboolean castShadow )
{
}

GLvoid Demx::Light::setShadowPosition( VEC3 pos, VEC3 up )
{
}

/** 
*	@brief Set the resolution of the light's shadow texture
* 
*	@author Demian Floreani
*
*	@param[in] resolution Value specifying resolution
*
*	@return None
*/
GLvoid Demx::Light::setShadowResolution( GLuint resolution )
{
	if( this->shadow != nullptr )
		this->shadow->setResolution( resolution );
}

/** 
*	@brief Query if light has shadow
* 
*	@author Demian Floreani
*
*	@return GLboolean If true the light has a shadow
*/
GLboolean Demx::Light::hasShadow()
{
	return shadow == nullptr ? GL_FALSE : GL_TRUE;
}

/** 
*	@brief Set the index of the shadow. This is used to store the shadow texture. Each shadow must have a unique index starting from 0.
* 
*	@author Demian Floreani
*
*	@param[in] index Value specifying the shadow index
*
*	@return None
*/
GLvoid Demx::Light::setShadowDepthIndex( GLuint index )
{
	if( this->shadow != nullptr )
		this->shadow->depthIndex = index;
}

/** 
*	@brief Get the shadow index
* 
*	@author Demian Floreani
*
*	@return GLint The shadow index
*/
GLint Demx::Light::getShadowDepthIndex()
{
	if( this->shadow != nullptr )
		return this->shadow->depthIndex;
	else
		return -1;
}

/** 
*	@brief Query if the light is active
* 
*	@author Demian Floreani
*
*	@return GLboolean If true the light is active
*/
GLboolean Demx::Light::isActive()
{
	return active;
}

/** 
*	@brief Set if the light is active
* 
*	@author Demian Floreani
*
*	@param[in] active Value
*
*	@return None
*/
GLvoid Demx::Light::setActive( GLboolean active )
{
	this->active = active;
}

/** 
*	@brief Constructor for an ambient light.
* 
*	@author Demian Floreani
*
*	@param[in] color COLOR3 representing the light's color
*
*	@return None
*/
Demx::AmbientLight::AmbientLight( COLOR3 color ) : Light( color )
{
}

/** 
*	@brief Constructor for a directional light.
* 
*	@author Demian Floreani
*
*	@param[in] color COLOR3 representing the light's color
*	@param[in] direction VEC3 representing the light's direction
*
*	@return None
*/
Demx::DirectionalLight::DirectionalLight( COLOR3 color, VEC3 direction ) : Light( color )
{
	this->direction = direction;
}

/** 
*	@brief Set if the light casts a shadow
* 
*	@author Demian Floreani
*
*	@param[in] castShadow Value
*
*	@return None
*/
void Demx::DirectionalLight::castShadow( GLboolean castShadow )
{
	if( castShadow )
	{
		if( shadow == nullptr )
		{
			shadow = new OrthoShadow();
			shadow->setCamera( -direction, direction, VEC3_Y );
		}
	}
	else
	{
		if( shadow != nullptr )
		{
			delete shadow;
			shadow = nullptr;
		}
	}
}

/** 
*	@brief Set the position of the shadow's camera. The render pass for the shadow will be from this position. 
* 
*	@author Demian Floreani
*
*	@param[in] pos The position
*	@param[in] up The up vector
*
*	@return None
*/
GLvoid Demx::DirectionalLight::setShadowPosition(VEC3 pos, VEC3 up )
{
	if( shadow != nullptr )	
		shadow->setCamera( pos, direction, up );
}

/** 
*	@brief Constructor for a directional light.
* 
*	@author Demian Floreani
*
*	@param[in] color COLOR3 representing the light's color
*	@param[in] position VEC3 representing the light's position
*
*	@return None
*/
Demx::PointLight::PointLight( COLOR3 color, VEC3 position ) : Light( color )
{
	this->position = position;
	
	this->constantAtt = 0.5f;
	this->linearAtt = 0.05f;
	this->expAtt = 0.005f;

	active = GL_TRUE;
}

/** 
*	@brief Set the position of the light
* 
*	@author Demian Floreani
*
*	@param[in] position VEC3 representing the light's position
*
*	@return None
*/
void Demx::PointLight::setPosition( VEC3 position )
{
	this->position = position;
}

/** 
*	@brief Set the attenuation values for the light
* 
*	@author Demian Floreani
*
*	@param[in] constant Constant attenuation value
*	@param[in] linear Linear attenuation value
*	@param[in] exp Exponential attenuation value
*
*	@return None
*/
void Demx::PointLight::setAttributes( GLfloat constant, GLfloat linear, GLfloat exp )
{
	this->constantAtt = constant;
	this->linearAtt = linear;
	this->expAtt = exp;
}

/** 
*	@brief Set if the light casts a shadow
* 
*	@author Demian Floreani
*
*	@param[in] castShadow Value
*
*	@return None
*/
void Demx::PointLight::castShadow( GLboolean castShadow )
{
	if( castShadow )
	{
		if( shadow == nullptr )
		{
			shadow = new ProjectionCubeShadow();
			shadow->setCamera( position, VEC3_ZERO, VEC3_ZERO );
		}
	}
	else
	{
		if( shadow != nullptr )
		{
			delete shadow;
			shadow = nullptr;
		}
	}
}

/** 
*	@brief Set the position of the shadow's camera. The render pass for the shadow will be from this position. 
* 
*	@author Demian Floreani
*
*	@param[in] pos The position
*	@param[in] up The up vector
*
*	@return None
*/
GLvoid Demx::PointLight::setShadowPosition(VEC3 pos, VEC3 up )
{
	if( shadow != nullptr )	
		shadow->setCamera( pos, VEC3_ZERO, VEC3_ZERO );
}

/** 
*	@brief Constructor for a spot light.
* 
*	@author Demian Floreani
*
*	@param[in] color COLOR3 representing the light's color
*	@param[in] position VEC3 representing the light's position
*	@param[in] direction VEC3 representing the light's direction
*	@param[in] angle The angle in degrees
*
*	@return None
*/
Demx::SpotLight::SpotLight( COLOR3 color, VEC3 position, VEC3 direction, GLfloat angle ) : Light( color )
{
	this->position = position;
	this->direction = direction;
	this->setAngle( angle );

	this->constantAtt = 0.2f;
	this->linearAtt = 0.01f;
	this->expAtt = 0.001f;

	active = true;
}

/** 
*	@brief Set the angle of the spot light
* 
*	@author Demian Floreani
*
*	@param[in] angle The angle in degrees
*
*	@return None
*/
void Demx::SpotLight::setAngle( GLfloat angle )
{
	this->coneCosine = glm::cos( glm::radians( angle ) );
}

/** 
*	@brief Set the position of the light
* 
*	@author Demian Floreani
*
*	@param[in] position VEC3 representing the light's position
*
*	@return None
*/
void Demx::SpotLight::setPosition( VEC3 position )
{
	this->position = position;
}

/** 
*	@brief Set the direction of the light
* 
*	@author Demian Floreani
*
*	@param[in] direction VEC3 representing the light's direction
*
*	@return None
*/
void Demx::SpotLight::setDirection( VEC3 view )
{
	this->direction = NORMALIZE(view - position);
}

/** 
*	@brief Set the attenuation values for the light
* 
*	@author Demian Floreani
*
*	@param[in] constant Constant attenuation value
*	@param[in] linear Linear attenuation value
*	@param[in] exp Exponential attenuation value
*
*	@return None
*/
void Demx::SpotLight::setAttributes( GLfloat constant, GLfloat linear, GLfloat exp )
{
	this->constantAtt = constant;
	this->linearAtt = linear;
	this->expAtt = exp;
}

/** 
*	@brief Set if the light casts a shadow
* 
*	@author Demian Floreani
*
*	@param[in] castShadow Value
*
*	@return None
*/
void Demx::SpotLight::castShadow( GLboolean castShadow )
{
	if( castShadow )
	{
		if( shadow == nullptr )
		{
			shadow = new ProjectionShadow();
			shadow->setCamera( position, direction, VEC3_Y );
		}
	}
	else
	{
		if( shadow != nullptr )
		{
			delete shadow;
			shadow = nullptr;
		}
	}
}

/** 
*	@brief Set the position of the shadow's camera. The render pass for the shadow will be from this position. 
* 
*	@author Demian Floreani
*
*	@param[in] pos The position
*	@param[in] up The up vector
*
*	@return None
*/
GLvoid Demx::SpotLight::setShadowPosition( VEC3 pos, VEC3 up )
{
	if( shadow != nullptr )
		shadow->setCamera( pos, direction, up );
}