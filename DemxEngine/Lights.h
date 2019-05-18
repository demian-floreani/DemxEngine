#pragma once

#include "Shadow.h"

namespace Demx
{
	/** 
	*	@brief Base class for the lights
	*/
	class Light
	{
		friend Renderer;

	public:
		Light( COLOR3 color );
		virtual ~Light();

		std::string name;
		COLOR3 color;
		VEC3 position;

		Shadow* shadow;
		GLboolean active;
	
	public:
		GLvoid setName( std::string name );

		virtual GLvoid castShadow( GLboolean castShadow );
		virtual GLvoid setShadowPosition( VEC3 pos, VEC3 up = VEC3_Y );
		GLvoid setShadowResolution( GLuint resolution );
		GLboolean hasShadow();
		
		GLvoid setShadowDepthIndex( GLuint index );
		GLint getShadowDepthIndex();

		GLboolean isActive();
		GLvoid setActive( GLboolean active );
		GLvoid toggle();
	};

	/** 
	*	@brief Represent a scene's ambient light. Derives from Demx::Light
	*/
	class AmbientLight : public Light
	{
	public:
		AmbientLight( COLOR3 color );
	};

	/** 
	*	@brief Represent a directional light which casts rays in one direction. Used usually to model the Sun.
	*   Derives from Demx::Light
	*/
	class DirectionalLight : public Light
	{
	public:
		VEC3 direction;

	public:
		DirectionalLight( COLOR3 color, VEC3 direction );

		GLvoid castShadow( GLboolean castShadow );
		GLvoid setShadowPosition( VEC3 pos, VEC3 up = VEC3_Y ) override;
	};

	/** 
	*	@brief Represent a spot light which casts rays in all directions.
	*   Derives from Demx::Light
	*/
	class PointLight : public Light
	{
	public:
		GLfloat constantAtt;
		GLfloat linearAtt;
		GLfloat expAtt;

	public:
		PointLight( COLOR3 color, VEC3 position );

		void setPosition( VEC3 position );
		void setAttributes( GLfloat constant, GLfloat linear, GLfloat exp );

		GLvoid castShadow( GLboolean castShadow );
		GLvoid setShadowPosition( VEC3 pos, VEC3 up = VEC3_Y ) override;
	};

	/** 
	*	@brief Represent a spot light which casts rays in all directions but within a specified angle.
	*   Derives from Demx::Light
	*/
	class SpotLight : public Light
	{
	public:
		VEC3 direction;

		GLfloat angle;
		GLfloat coneCosine;
		GLfloat constantAtt;
		GLfloat linearAtt;
		GLfloat expAtt;

	public:
		SpotLight( COLOR3 color, VEC3 position, VEC3 direction, GLfloat angle );

		void setAngle( GLfloat angle );
		void setPosition( VEC3 position );
		void setDirection( VEC3 view );
		void setAttributes( GLfloat constant, GLfloat linear, GLfloat exp );

		GLvoid castShadow( GLboolean castShadow );
		GLvoid setShadowPosition( VEC3 pos, VEC3 up = VEC3_Y ) override;
	};

	typedef struct 
	{
		Demx::AmbientLight* ambientLight;
		std::vector<Demx::DirectionalLight*> directionalLights;
		std::vector<Demx::PointLight*> pointLights;
		std::vector<Demx::SpotLight*> spotLights;
	} Lights, *PLIGHTS;
}
