#include "stdafx.h"
#include "Entity.h"

Demx::Entity* Demx::Entity::createCube( std::string material, GLint w, GLint h )
{
	Entity* cube = new Entity( GL_TRIANGLES );
	
	// front
	cube->position( glm::vec3( -1.0f,  1.0f, 1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f, 1.0f ) );
	cube->position( glm::vec3(  1.0f, -1.0f, 1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f ) );
	cube->setFaceNormal( VEC3_Z );

	cube->position( glm::vec3( -1.0f,  1.0f, 1.0f ) );
	cube->position( glm::vec3(  1.0f, -1.0f, 1.0f ) );
	cube->position( glm::vec3(  1.0f,  1.0f, 1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( VEC3_Z );

	// back
	cube->position( glm::vec3(  1.0f,  1.0f, -1.0f ) );
	cube->position( glm::vec3(  1.0f, -1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f,  1.0f, -1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f * w, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( -VEC3_Z );

	cube->position( glm::vec3( -1.0f,  1.0f, -1.0f ) );
	cube->position( glm::vec3(  1.0f, -1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f, -1.0f ) );
	cube->map( glm::vec2( 1.0f * h, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f ) );
	cube->setFaceNormal( -VEC3_Z );

	// left
	cube->position( glm::vec3( -1.0f,  1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f,  1.0f,  1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( -VEC3_X );
	
	cube->position( glm::vec3( -1.0f,  1.0f,  1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f,  1.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f * h ) );
	cube->setFaceNormal( -VEC3_X );

	// right
	cube->position( glm::vec3( 1.0f,  1.0f,  1.0f ) );
	cube->position( glm::vec3( 1.0f, -1.0f,  1.0f ) );
	cube->position( glm::vec3( 1.0f,  1.0f, -1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( VEC3_X );

	cube->position( glm::vec3( 1.0f,  1.0f, -1.0f ) );
	cube->position( glm::vec3( 1.0f, -1.0f,  1.0f ) );
	cube->position( glm::vec3( 1.0f, -1.0f, -1.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f * h ) );
	cube->setFaceNormal( VEC3_X );

	// bottom
	cube->position( glm::vec3( -1.0f, -1.0f,  1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f,  -1.0f ) );
	cube->position( glm::vec3( 1.0f, -1.0f, 1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( -VEC3_Y );

	cube->position( glm::vec3( 1.0f, -1.0f,  1.0f ) );
	cube->position( glm::vec3( -1.0f, -1.0f,  -1.0f ) );
	cube->position( glm::vec3( 1.0f, -1.0f, -1.0f ) );
	cube->map( glm::vec2( 1.0f*w, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f ) );
	cube->setFaceNormal( -VEC3_Y );

	// top
	cube->position( glm::vec3( -1.0f, 1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f, 1.0f,  1.0f ) );
	cube->position( glm::vec3(  1.0f, 1.0f, -1.0f ) );
	cube->map( glm::vec2( 0.0f, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->setFaceNormal( VEC3_Y );

	cube->position( glm::vec3(  1.0f, 1.0f, -1.0f ) );
	cube->position( glm::vec3( -1.0f, 1.0f,  1.0f ) );
	cube->position( glm::vec3(  1.0f, 1.0f,  1.0f ) );
	cube->map( glm::vec2( 1.0f * w, 1.0f * h ) );
	cube->map( glm::vec2( 0.0f, 0.0f ) );
	cube->map( glm::vec2( 1.0f * w, 0.0f ) );
	cube->setFaceNormal( VEC3_Y );

	cube->setMaterial( material );
	return cube;
}