#pragma once

#include "Shadow.h"

namespace Demx
{
	class ProjectionShadow :public Shadow
	{
	public:
		ProjectionShadow( GLuint resolution = MEDIUM_RESOLUTION );
		~ProjectionShadow(void);

		GLvoid setCamera( VEC3 pos, VEC3 dir, VEC3 up ) override;
	};
}
