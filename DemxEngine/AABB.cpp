#include "stdafx.h"

#include "AABB.h"

/** 
*	@brief AABB constructor
* 
*	@author Demian Floreani
*
*   @return None
*/
Demx::AABB::AABB()
{
	reset();
	vao = 0;
}

Demx::AABB::~AABB()
{
}

/** 
*	@brief Reset the AABB
* 
*	@author Demian Floreani
*
*   @return None
*/
GLvoid Demx::AABB::reset()
{
	min.x = min.y = min.z = FLT_MAX;
	max.x = max.y = max.z = -FLT_MAX;
}

/** 
*	@brief Used while constructing the AABB. Add a point and check it with the current min and max values.
*
*	@author Demian Floreani
*
*	@param[in] p VEC3 representing a point
*
*   @return None
*/
GLvoid Demx::AABB::add( const VEC3& p )
{
	if( p.x < min.x ) 
		min.x = p.x;
	if( p.x > max.x ) 
		max.x = p.x;

	if( p.y < min.y ) 
		min.y = p.y;
	if( p.y > max.y ) 
		max.y = p.y;

	if( p.z < min.z )
		min.z = p.z;
	if( p.z > max.z ) 
		max.z = p.z;
}

/** 
*	@brief Used while constructing the AABB. Add a point and check it with the current min and max values.
*
*	@author Demian Floreani
*
*	@param[in] p VEC4 representing a point
*
*   @return None
*/
GLvoid Demx::AABB::add( const VEC4& p )
{
	if( p.x < min.x ) 
		min.x = p.x;
	if( p.x > max.x ) 
		max.x = p.x;

	if( p.y < min.y ) 
		min.y = p.y;
	if( p.y > max.y ) 
		max.y = p.y;

	if( p.z < min.z )
		min.z = p.z;
	if( p.z > max.z ) 
		max.z = p.z;
}

/** 
*	@brief Set the 8 bounds of the AABB based on min and max values.
*
*	@author Demian Floreani
*
*   @return None
*/
GLvoid Demx::AABB::setBounds()
{
	bounds[0] = VEC3( max.x, max.y, min.z );
	bounds[1] = VEC3( min.x, max.y, min.z );
	bounds[2] = VEC3( min.x, min.y, min.z );
	bounds[3] = VEC3( max.x, min.y, min.z );
	bounds[4] = VEC3( max.x, min.y, max.z );
	bounds[5] = VEC3( max.x, max.y, max.z );
	bounds[6] = VEC3( min.x, max.y, max.z );
	bounds[7] = VEC3( min.x, min.y, max.z );
}

/** 
*	@brief Transform the AABB based on a MAT4 matrix
*
*	@author Demian Floreani
*
*	@param[in] t The transformation matrix
*
*   @return None
*/
GLvoid Demx::AABB::transform( const MAT4& t )
{
	VEC3 transBounds[8];

	for( GLint i = 0; i < 8; ++i )
	{
		VEC4 bound = t * VEC4( bounds[i], 1.0f );
		transBounds[i] = VEC3( bound.x, bound.y, bound.z );
	}

	reset();

	for( GLint i = 0; i < 8; ++i )
	{
		add( transBounds[i] );
	}

	setCenter();
}

/** 
*	@brief Setup the AABB using a vector of mesh vertices
*
*	@author Demian Floreani
*
*	@param[in] vertices Vector of vertices
*
*   @return None
*/
GLvoid Demx::AABB::setup( const std::vector<VEC3>& vertices )
{
	reset();

	for(	std::vector<glm::vec3>::const_iterator vertex = vertices.begin();
			vertex != vertices.end();
			++vertex )
	{
		add( *vertex );
	}
		
	setBounds();

	setCenter();
}

/** 
*	@brief Setup the AABB using a vector of mesh vertices
*
*	@author Demian Floreani
*
*	@param[in] vertices Vector of vertices
*
*   @return None
*/
GLvoid Demx::AABB::setup( const std::vector<VEC4>& vertices )
{
	reset();

	for(	std::vector<glm::vec4>::const_iterator vertex = vertices.begin();
			vertex != vertices.end();
			++vertex )
	{
		add( *vertex );
	}
		
	setBounds();

	setCenter();
}

/** 
*	@brief Get the positive vertex of the AABB based on plane's normal
*
*	@author Demian Floreani
*
*	@param[in] normal The plane normal
*
*   @return VEC3 Positive vertex
*/
VEC3 Demx::AABB::getPositiveVertex( const VEC3& normal ) const
{
	VEC3 p = min;

	if (normal.x >= 0)
	{
		p.x = max.x;
	}
	if (normal.y >=0)
	{
		p.y = max.y;
	}
	if (normal.z >= 0)
	{
		p.z = max.z;
	}

	return p;
}

/** 
*	@brief Get the negative vertex of the AABB based on plane's normal
*
*	@author Demian Floreani
*
*	@param[in] normal The plane normal
*
*   @return VEC3 Negative vertex
*/
VEC3 Demx::AABB::getNegativeVertex( const VEC3& normal ) const
{
	VEC3 p = max;

	if (normal.x >= 0)
	{
		p.x = min.x;
	}
	if (normal.y >=0)
	{
		p.y = min.y;
	}
	if (normal.z >= 0)
	{
		p.z = min.z;
	}

	return p;
}

/** 
*	@brief Get the center of the AABB
*
*	@author Demian Floreani
*
*   @return VEC3 Center of AABB
*/
VEC3& Demx::AABB::getCenter()
{
	return center;
}

/** 
*	@brief Set the center of the AABB
*
*	@author Demian Floreani
*
*   @return None
*/
GLvoid Demx::AABB::setCenter()
{
	center = (min + max) / 2.0f;
}

GLvoid Demx::AABB::setupLines()
{
	VEC3 drawBounds[8];

	drawBounds[0] = VEC3( max.x, max.y, min.z );
	drawBounds[1] = VEC3( min.x, max.y, min.z );
	drawBounds[2] = VEC3( min.x, min.y, min.z );
	drawBounds[3] = VEC3( max.x, min.y, min.z );
	drawBounds[4] = VEC3( max.x, min.y, max.z );
	drawBounds[5] = VEC3( max.x, max.y, max.z );
	drawBounds[6] = VEC3( min.x, max.y, max.z );
	drawBounds[7] = VEC3( min.x, min.y, max.z );

	VEC3 vertices[] = { 
		drawBounds[0], drawBounds[1],
		drawBounds[0], drawBounds[5],
		drawBounds[5], drawBounds[6],
		drawBounds[1], drawBounds[6],
		drawBounds[1], drawBounds[2],
		drawBounds[0], drawBounds[3],
		drawBounds[5], drawBounds[4],
		drawBounds[6], drawBounds[7],
		drawBounds[2], drawBounds[3],
		drawBounds[3], drawBounds[4],
		drawBounds[4], drawBounds[7],
		drawBounds[2], drawBounds[7],
	};

	GLuint vbo;
	glGenVertexArrays( 1, &vao ); 
	glGenBuffers( 1, &vbo );

	glBindVertexArray( vao );

	glBindBuffer(GL_ARRAY_BUFFER, vbo );
	glBufferData(GL_ARRAY_BUFFER,  24 * sizeof(GLfloat) * 3, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

GLvoid Demx::AABB::drawLines()
{
	glBindVertexArray( vao );
	glDrawArrays( GL_LINES, 0, 24 );
}