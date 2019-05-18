#pragma once

#include "Entity.h"

#include <deque>

namespace Demx
{
	class RenderQueue
	{
	public:
		std::deque<Entity*> opaque;
		std::deque<Entity*> transparent;

	public:
		RenderQueue(void);
		~RenderQueue(void);

		GLvoid empty();

		GLvoid pushOpaque( Entity* element );
		GLvoid pushTransparent( Entity* element );

		GLvoid push( Entity* element );

		GLvoid sort();
	};
}