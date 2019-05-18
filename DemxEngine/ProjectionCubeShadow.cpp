#include "stdafx.h"

#include "ProjectionCubeShadow.h"

/** 
*	@brief Constructor for a cube projection shadow. This type of shadow is used for point lights where projection is necessary.
* 
*	@author Demian Floreani
*
 *	@param[in] resolution The resolution in pixels of the shadow map texture. A higher value will give a more detailed shadow, but will impact performance.
*
*	@return None
*/
Demx::ProjectionCubeShadow::ProjectionCubeShadow( GLuint resolution ) : Shadow( resolution )
{
	depthViewMatrix = new MAT4[6];
	depthViewProjMatrix = new MAT4[6];
	frustum = new Demx::Frustum[6];
	queue = new Demx::RenderQueue[6];

	depthProjectionMatrix = glm::perspective<GLfloat>( 90.0f, 1.0f, 1.0f, 100.0f );
}

Demx::ProjectionCubeShadow::~ProjectionCubeShadow(void)
{
	delete[] depthViewMatrix;
	delete[] depthViewProjMatrix;
	delete[] frustum;
	delete[] queue;
}

/** 
*	@brief Set the projection matrix and frustum for rendering the scene from the shadow's point of view. Each side needs it's own 
*   projection matrix and frustum.
* 
*	@author Demian Floreani
*
*	@param[in] pos VEC3 specifying the position of the camera
*	@param[in] dir VEC3 specifying the direction
*	@param[in] up VEC3 specifying the up vector
*
*	@return None
*/
void Demx::ProjectionCubeShadow::setCamera( VEC3 pos, VEC3 dir, VEC3 up )
{
	position = pos;

	depthViewMatrix[0] = glm::lookAt( pos, pos + VEC3_X, -VEC3_Y );
	depthViewMatrix[1] = glm::lookAt( pos, pos - VEC3_X, -VEC3_Y );
	depthViewMatrix[2] = glm::lookAt( pos, pos + VEC3_Y,  VEC3_Z );
	depthViewMatrix[3] = glm::lookAt( pos, pos - VEC3_Y, -VEC3_Z );
	depthViewMatrix[4] = glm::lookAt( pos, pos + VEC3_Z, -VEC3_Y );
	depthViewMatrix[5] = glm::lookAt( pos, pos - VEC3_Z, -VEC3_Y );

	for( GLint i = 0; i < 6; ++i )
	{
		depthViewProjMatrix[i] = depthProjectionMatrix * depthViewMatrix[i];
		frustum[i].set( depthViewProjMatrix[i] );
	} 
}
