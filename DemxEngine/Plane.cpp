#include "stdafx.h"

#include "Plane.h"

Demx::Plane::Plane()
{
}

Demx::Plane::~Plane()
{
}

/** 
*	@brief Sets the plane values
* 
*	@author Demian Floreani
*
*   @param[in] normal The plane normal
*	@param[in] d The plane d value
*
*	@return None
*/
GLvoid Demx::Plane::set( VEC3 normal, GLfloat d )
{
	GLfloat length = glm::length( normal );
	this->normal = NORMALIZE( normal );
	this->d = d / length;
}

/** 
*	@brief Calculate distance between a point and the plane
*   The formula used is: dist = n . p + d
*
*	@author Demian Floreani
*
*	@return GLfloat distance
*/
GLfloat Demx::Plane::calcDistance( VEC3 p ) const
{
	return glm::dot( normal, p ) + d;
}