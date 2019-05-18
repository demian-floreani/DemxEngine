#include "stdafx.h"
#include "Entity.h"

#define _USE_MATH_DEFINES

#include <math.h>

Demx::Entity* Demx::Entity::createCircle( std::string material, GLfloat radius, GLint w, GLint h )
{
	Demx::Entity* circle = new Entity( GL_TRIANGLE_FAN );

	circle->position( VEC3( 0.0f, 0.0f, 0.0f ) );
	circle->map( VEC2( 0.5f, 0.5f ) );
	circle->normal( VEC3( VEC3_Z ) );

	for(GLdouble angle = 0; angle <= 360.0f; ++angle )
	{
		GLdouble rad = angle * M_PI / 180.0f;

		circle->position( VEC3( glm::cos( rad ) * radius, glm::sin( rad ) * radius, 0.0f ) );
		circle->map( VEC2( ( glm::cos( rad ) + 1.0f ) * 0.5f * (GLfloat)w, ( glm::sin( rad ) + 1.0f ) * 0.5f * (GLfloat)h ) );
		circle->normal( VEC3( VEC3_Z ) );
	}
	
	circle->setMaterial( material );

	return circle;
}
