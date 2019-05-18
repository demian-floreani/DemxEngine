#pragma once

namespace Demx
{
	/** 
	*	@brief Represent a Axis Aligned Bounding Box using min and max points.
	*/
	class AABB
	{
		VEC3 min;
		VEC3 max;
		VEC3 center;

		VEC3 bounds[8];

		GLuint vao;

	public:
		AABB();
		~AABB();

		GLvoid reset();

		GLvoid add( const VEC3& p );
		GLvoid add( const VEC4& p );

		GLvoid setBounds();

		GLvoid setup( const std::vector<VEC3>& vertices );
		GLvoid setup( const std::vector<VEC4>& vertices );

		GLvoid transform( const MAT4& t );

		VEC3& getCenter();
		GLvoid setCenter();

		VEC3 getPositiveVertex( const VEC3& normal ) const;
		VEC3 getNegativeVertex( const VEC3& normal ) const;

		GLvoid setupLines();
		GLvoid drawLines();
	};
}