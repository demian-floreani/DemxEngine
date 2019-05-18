#include "stdafx.h"
#include "Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>

Demx::Entity* Demx::Entity::createCylinder( std::string material, GLfloat radius, GLfloat height, GLint w, GLint h )
{
	Demx::Entity* cylinder = new Entity( GL_TRIANGLES );

	//top circle
	for(GLdouble angle = 0; angle <= 360.0; ++angle )
	{
		GLdouble rad = angle * M_PI / 180.0f;
		GLdouble nextRad = (angle + 1.0) * M_PI / 180.0f;

		cylinder->position( VEC3( 0.0f, height, 0.0f ) );
		cylinder->map( VEC2( 0.5f, 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );

		cylinder->position( VEC3( glm::cos( rad ) * radius, height, -glm::sin( rad ) * radius ) );
		cylinder->map( VEC2( ( glm::cos( rad ) + 1.0f ) * 0.5f, ( glm::sin( rad ) + 1.0f ) * 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );

		cylinder->position( VEC3( glm::cos( nextRad ) * radius, height, -glm::sin( nextRad ) * radius ) );
		cylinder->map( VEC2( ( glm::cos( nextRad ) + 1.0f ) * 0.5f, ( glm::sin( nextRad ) + 1.0f ) * 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );
	}

	// side
	// texture mapping
	// s = RADIANS / 2 * PI

	for(GLdouble angle = 0; angle <= 360.0; ++angle )
	{
		GLdouble rad = angle * M_PI / 180.0f;
		GLdouble nextRad = (angle + 1.0) * M_PI / 180.0f;

		cylinder->triangle(	VEC3(glm::cos( rad ) * radius, height, -glm::sin( rad ) * radius), 
							VEC3(glm::cos( rad ) * radius, 0.0f, -glm::sin( rad ) * radius), 
							VEC3(glm::cos( nextRad ) * radius, height, -glm::sin( nextRad ) * radius) );

		cylinder->map( VEC2( ( rad / 2 * M_PI ) * (double)w, 1.0f * h ) );
		cylinder->map( VEC2( ( rad / 2 * M_PI ) * (double)w, 0.f ) );
		cylinder->map( VEC2( ( nextRad / 2 * M_PI ) * (double)w, 1.0f * h ) );

		cylinder->triangle(	VEC3(glm::cos( nextRad ) * radius, height, -glm::sin( nextRad ) * radius), 
							VEC3(glm::cos( rad ) * radius, 0.0f, -glm::sin( rad ) * radius), 
							VEC3(glm::cos( nextRad ) * radius, 0.0f, -glm::sin( nextRad ) * radius) );

		cylinder->map( VEC2( ( nextRad / 2 * M_PI ) * (double)w, 1.0f * h ) );
		cylinder->map( VEC2( ( rad / 2 * M_PI ) * (double)w, 0.f ) );
		cylinder->map( VEC2( ( nextRad / 2 * M_PI ) * (double)w, 0.f ) );
	}

	//bottom circle
	for(GLdouble angle = 0; angle <= 360.0; ++angle )
	{
		GLdouble rad = angle * M_PI / 180.0f;
		GLdouble nextRad = (angle + 1.0) * M_PI / 180.0f;

		cylinder->position( VEC3( 0.0f, 0.0f, 0.0f ) );
		cylinder->map( VEC2( 0.5f, 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );

		cylinder->position( VEC3( glm::cos( rad ) * radius, 0.0f, glm::sin( rad ) * radius ) );
		cylinder->map( VEC2( ( glm::cos( rad ) + 1.0f ) * 0.5f, ( glm::sin( rad ) + 1.0f ) * 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );

		cylinder->position( VEC3( glm::cos( nextRad ) * radius, 0.0f, glm::sin( nextRad ) * radius ) );
		cylinder->map( VEC2( ( glm::cos( nextRad ) + 1.0f ) * 0.5f, ( glm::sin( nextRad ) + 1.0f ) * 0.5f ) );
		cylinder->normal( VEC3( VEC3_Y ) );
	}
	
	cylinder->setMaterial( material );

	return cylinder;
}