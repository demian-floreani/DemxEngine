#include "stdafx.h"
#include "OrthoShadow.h"

/** 
*	@brief Constructor for an orthogonal shadow. This type of shadow is used for directional lights.
* 
*	@author Demian Floreani
*
 *	@param[in] resolution The resolution in pixels of the shadow map texture. A higher value will give a more detailed shadow, but will impact performance.
*
*	@return None
*/
Demx::OrthoShadow::OrthoShadow( GLuint resolution ) : Shadow( resolution )
{
	depthViewMatrix = new MAT4(1);
	depthViewProjMatrix = new MAT4(1);
	frustum = new Demx::Frustum();
	queue = new Demx::RenderQueue();

	depthProjectionMatrix = glm::ortho<GLfloat>( -15.0f, 15.0f, -15.0f, 15.0f, 1.0f, 100.0f );
}

Demx::OrthoShadow::~OrthoShadow(void)
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
void Demx::OrthoShadow::setCamera( VEC3 pos, VEC3 dir, VEC3 up )
{
	position = pos;

	*depthViewMatrix = glm::lookAt( pos, pos + dir, up );
	*depthViewProjMatrix = depthProjectionMatrix * (*depthViewMatrix);

	frustum->set( *depthViewProjMatrix );
}
