#pragma once

namespace Demx
{
	/** 
	*	@brief Class to mathematically represent a plane.
	*/
	class Plane
	{
	public:
		VEC3 normal;
		GLfloat d;

		Plane();
		~Plane();

		GLvoid set( VEC3 normal, GLfloat d );
		GLfloat calcDistance( VEC3 p ) const;
	};
}