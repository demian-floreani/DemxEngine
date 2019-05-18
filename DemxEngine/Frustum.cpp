#include "stdafx.h"
#include "Frustum.h"

Demx::Frustum::Frustum()
{
}

Demx::Frustum::~Frustum()
{
}

/** 
*	@brief Sets the frustum planes by passing a view matrix
*   Method used found in section 5.5.3 of Mathematics for 3D Game Programming and CG
*
*	@author Demian Floreani
*
*   @param[in] mat The view matrix
*
*	@return None
*/
GLvoid Demx::Frustum::set( MAT4 mat )
{
	planes[ NEAR_PLANE ].set(	VEC3(	mat[0][3] + mat[0][2], 
										mat[1][3] + mat[1][2], 
										mat[2][3] + mat[2][2] ), 
								mat[3][3] + mat[3][2] );
		
	planes[ FAR_PLANE ].set(	VEC3(	mat[0][3] - mat[0][2], 
										mat[1][3] - mat[1][2], 
										mat[2][3] - mat[2][2] ), 
								mat[3][3] - mat[3][2] );

	planes[ LEFT_PLANE ].set(	VEC3(	mat[0][3] + mat[0][0], 
										mat[1][3] + mat[1][0], 
										mat[2][3] + mat[2][0] ), 
								mat[3][3] + mat[3][0] );

	planes[ RIGHT_PLANE ].set(	VEC3(	mat[0][3] - mat[0][0], 
										mat[1][3] - mat[1][0], 
										mat[2][3] - mat[2][0] ), 
								mat[3][3] - mat[3][0] );

	planes[ BOTTOM_PLANE ].set(	VEC3(	mat[0][3] + mat[0][1], 
										mat[1][3] + mat[1][1], 
										mat[2][3] + mat[2][1] ), 
								mat[3][3] + mat[3][1] );

	planes[ TOP_PLANE ].set(	VEC3(	mat[0][3] - mat[0][1], 
										mat[1][3] - mat[1][1], 
										mat[2][3] - mat[2][1] ), 
								mat[3][3] - mat[3][1] );
}

/** 
*	@brief Check if a AABB intersects the frustum.
*
*	@author Demian Floreani
*
*   @param[in] box AABB to check
*
*	@return GLint Result of intersection check. If 0 there is no intersection.
*/
GLint Demx::Frustum::AABBIntersect( const Demx::AABB& box ) const
{
	// set AABB as inside the plane
	GLint res = IN_VIEW;

	// loop the view frustum planes
	for( GLint i = 0; i < 6; ++i )
	{
		// If the distance is negative the AABB is on the other side of the plane (based on the plane normal) so outside the frustum
		if( planes[i].calcDistance( box.getPositiveVertex( planes[i].normal ) ) < 0 )
		{
			// AABB is outside the plane
			return OUTSIDE_VIEW; 
		}
		else if( planes[i].calcDistance( box.getNegativeVertex( planes[i].normal ) ) < 0 )
		{
			// AABB intersets the plane
			res = INTERSECT_VIEW; 
		}
	}

	return res;
}
