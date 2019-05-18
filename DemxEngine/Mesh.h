#pragma once

namespace Demx
{
	class Mesh
	{
	public:
		GLuint startIndices;
		GLuint numIndices;
		GLuint materialIndex;

		Mesh();
		~Mesh();
	};
}
