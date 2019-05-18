#pragma once

#include "Shadow.h"

namespace Demx
{
	class OrthoShadow :public Shadow
	{
	public:
		OrthoShadow( GLuint resolution = MEDIUM_RESOLUTION );
		~OrthoShadow(void);

		GLvoid setCamera( VEC3 pos, VEC3 dir, VEC3 up ) override;
	};
}