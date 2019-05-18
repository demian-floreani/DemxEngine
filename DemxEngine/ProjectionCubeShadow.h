#pragma once

#include "Shadow.h"

namespace Demx
{
	class ProjectionCubeShadow :public Shadow
	{
	public:
		ProjectionCubeShadow( GLuint resolution = LOW_RESOLUTION );
		~ProjectionCubeShadow(void);

		GLvoid setCamera( VEC3 pos, VEC3 dir, VEC3 up ) override;
	};
}
