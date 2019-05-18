#pragma once

#include "Plane.h"
#include "AABB.h"

#define OUTSIDE_VIEW	0
#define IN_VIEW			1
#define INTERSECT_VIEW	2

#define NEAR_PLANE		0
#define FAR_PLANE		1
#define LEFT_PLANE		2
#define RIGHT_PLANE		3
#define BOTTOM_PLANE	4
#define TOP_PLANE		5

namespace Demx 
{
	/** 
	*	@brief Represent a view frustum using 6 planes
	*/
	class Frustum
	{
	private:
		Demx::Plane planes[6];

	public:
		Frustum(GLvoid);
		~Frustum(GLvoid);

		GLvoid set( MAT4 mat );
		GLint AABBIntersect( const Demx::AABB& box ) const;
	};
}
