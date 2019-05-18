#include "stdafx.h"
#include "Entity.h"

Demx::Entity* Demx::Entity::createPlane( std::string material, GLfloat size, GLint w, GLint h )
{
	Entity* plane = new Entity( GL_TRIANGLE_STRIP );

	plane->position( VEC3( -size, 0.0f, -size ) );
	plane->position( VEC3( -size, 0.0f, size ) );
	plane->position( VEC3( size, 0.0f, -size ) );
	plane->position( VEC3( size, 0.0f, size ) );

	plane->map( VEC2( 1.0f * w, 0.0f ) );
	plane->map( VEC2( 0.0f, 0.0f ) );
	plane->map( VEC2( 1.0f * w, 1.0f * h) );
	plane->map( VEC2( 1.0f * w, 0.0f ) );

	plane->setFaceNormal( VEC3_Y );
	plane->setMaterial( material );

	return plane;
}