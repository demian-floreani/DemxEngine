#include "stdafx.h"
#include "Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>

Demx::Entity* Demx::Entity::createCone( std::string material, GLfloat radius, GLfloat height, GLint w, GLint h )
{
	Demx::Entity* cone = new Entity( GL_TRIANGLES );

	for(GLdouble angle = 0; angle <= 360.0f; ++angle )
	{
		GLdouble rad = angle * M_PI / 180.0f;
		GLdouble nextRad = (angle + 1.0) * M_PI / 180.0f;

		cone->triangle(		height * VEC3_Y, 
							VEC3(glm::cos( rad ) * radius, 0.0f, -glm::sin( rad ) * radius), 
							VEC3(glm::cos( nextRad ) * radius, 0.0f, -glm::sin( nextRad ) * radius) 
						);

		cone->map( VEC2( 0.5f, 0.5f ) );
		cone->map( VEC2( ( glm::cos( rad ) + 1.0f ) * 0.5f * (GLfloat)w, ( glm::sin( rad ) + 1.0f ) * 0.5f * (GLfloat)h ) );
		cone->map( VEC2( ( glm::cos( nextRad ) + 1.0f ) * 0.5f * (GLfloat)w, ( glm::sin( nextRad ) + 1.0f ) * 0.5f * (GLfloat)h ) );
	}

	cone->setMaterial( material );

	return cone;
}