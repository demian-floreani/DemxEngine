#include "stdafx.h"

#include "ProjectionShadow.h"

/** 
*	@brief Constructor for a projection shadow. This type of shadow is used for spot lights where projection is necessary.
* 
*	@author Demian Floreani
*
 *	@param[in] resolution The resolution in pixels of the shadow map texture. A higher value will give a more detailed shadow, but will impact performance.
*
*	@return None
*/
Demx::ProjectionShadow::ProjectionShadow( GLuint resolution ) : Shadow( resolution )
{
	depthViewMatrix = new IDENTITY_MAT4;
	depthViewProjMatrix = new IDENTITY_MAT4;
	frustum = new Demx::Frustum();
	queue = new Demx::RenderQueue();

	depthProjectionMatrix = glm::perspective<GLfloat>( 45.0f, 1.0f, 1.0f, 100.0f );
}

Demx::ProjectionShadow::~ProjectionShadow(void)
{
	delete depthViewMatrix;
	delete depthViewProjMatrix;
	delete frustum;
	delete queue;
}

/** 
*	@brief Set the projection matrix and frustum for rendering the scene from the shadow's point of view.
* 
*	@author Demian Floreani
*
*	@param[in] pos VEC3 specifying the position of the camera
*	@param[in] dir VEC3 specifying the direction
*	@param[in] up VEC3 specifying the up vector
*
*	@return None
*/
GLvoid Demx::ProjectionShadow::setCamera( VEC3 pos, VEC3 dir, VEC3 up )
{
	position = pos;

	*depthViewMatrix = glm::lookAt( pos, pos + dir, up );
	*depthViewProjMatrix = depthProjectionMatrix * (*depthViewMatrix);

	frustum->set( *depthViewProjMatrix );
}
