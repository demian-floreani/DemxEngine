#include "stdafx.h"
#include "Entity.h"

Demx::Entity* Demx::Entity::createPyramid( std::string material, GLint w, GLint h )
{
	Demx::Entity* pyramid = new Entity( GL_TRIANGLES );

	pyramid->triangle( VEC3( -1.0f, 0.0f, 1.0f ), VEC3( 1.0f, 0.0f, 1.0f ), VEC3( 0.0f, 1.0f, 0.0f ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 0.0f ) );
	pyramid->map( VEC2( 0.5f * w, 1.0f * h) );

	pyramid->triangle( VEC3( 1.0f, 0.0f, 1.0f ), VEC3( 1.0f, 0.0f, -1.0f ), VEC3( 0.0f, 1.0f, 0.0f ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 0.0f ) );
	pyramid->map( VEC2( 0.5f * w, 1.0f * h) );

	pyramid->triangle( VEC3( 1.0f, 0.0f, -1.0f ), VEC3( -1.0f, 0.0f, -1.0f ), VEC3( 0.0f, 1.0f, 0.0f ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 0.0f ) );
	pyramid->map( VEC2( 0.5f * w, 1.0f * h) );

	pyramid->triangle( VEC3( -1.0f, 0.0f, -1.0f ), VEC3( -1.0f, 0.0f, 1.0f ), VEC3( 0.0f, 1.0f, 0.0f ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 0.0f ) );
	pyramid->map( VEC2( 0.5f * w, 1.0f * h) );

	// bottom

	pyramid->triangle( VEC3( -1.0f, 0.0f, 1.0f ), VEC3( -1.0f, 0.0f, -1.0f ), VEC3( 1.0f, 0.0f, 1.0f ) );
	pyramid->map( VEC2( 0.0f, 1.0f * h ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 1.0f * h) );

	pyramid->triangle( VEC3( 1.0f, 0.0f, 1.0f ), VEC3( -1.0f, 0.0f, -1.0f ), VEC3( 1.0f, 0.0f, -1.0f ) );
	pyramid->map( VEC2( 1.0f * w, 1.0f * h ) );
	pyramid->map( VEC2( 0.0f, 0.0f ) );
	pyramid->map( VEC2( 1.0f * w, 0.0f ) );

	pyramid->setMaterial( material );
	return pyramid;
}