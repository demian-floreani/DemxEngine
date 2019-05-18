#include "stdafx.h"

#include "RenderQueue.h"

#include <algorithm>

Demx::RenderQueue::RenderQueue(void)
{
}

Demx::RenderQueue::~RenderQueue(void)
{
}

GLvoid Demx::RenderQueue::empty()
{
	opaque.clear();
	transparent.clear();
}

GLvoid Demx::RenderQueue::pushOpaque( Entity* element )
{
	opaque.push_back( element );
}

GLvoid Demx::RenderQueue::pushTransparent( Entity* element )
{
	transparent.push_back( element );
}

GLvoid Demx::RenderQueue::push( Demx::Entity* element )
{
	if( element->hasTransparency == GL_FALSE )
	{
		opaque.push_back( element );
	}
	else
	{
		transparent.push_back( element );
	}
}

GLvoid Demx::RenderQueue::sort()
{
	// ascending order, front to back
	std::sort(	opaque.begin(), 
				opaque.end(), 
				[](Demx::Entity* a, Demx::Entity* b)
				{
					return a->distance < b->distance;
				} 
	);

	// descending order, back to front
	std::sort(	transparent.begin(), 
				transparent.end(), 
				[](Demx::Entity* a, Demx::Entity* b)
				{
					return a->distance > b->distance;
				} 
	);
}
