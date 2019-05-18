#pragma once

#include "Frustum.h"
#include "RenderQueue.h"

#define LOW_RESOLUTION 1024
#define MEDIUM_RESOLUTION 2048
#define HIGH_RESOLUTION 4096

namespace Demx
{
	class Renderer;

	class Shadow
	{
		friend Renderer;

	protected:
		GLuint framebuffer;
		GLuint depthTexture;

		// used for texture cube maps
		GLuint cubeMap;

		MAT4 depthProjectionMatrix;
		MAT4* depthViewMatrix;
		MAT4* depthViewProjMatrix;

		GLuint resolution;

		// position of shadow camera
		VEC3 position;

		Demx::Frustum* frustum;
		Demx::RenderQueue* queue;

	public:
		GLuint depthIndex;
		GLint depthCubeIndex;

		Shadow( GLuint resolution = LOW_RESOLUTION );
		virtual ~Shadow(void);

		/** 
		 *	@brief Set the projection matrix for rendering the scene from the shadow's point of view. Must be implemented by derived classes.
		 * 
		 *	@author Demian Floreani
		 *
		 *	@param[in] pos VEC3 specifying the position of the camera
		 *	@param[in] dir VEC3 specifying the direction
		 *	@param[in] up VEC3 specifying the up vector
		 *
		 *	@return None
		 */
		virtual GLvoid setCamera( VEC3 pos, VEC3 dir, VEC3 up ) = 0;

		GLvoid setResolution( GLuint resolution );
	};
}
