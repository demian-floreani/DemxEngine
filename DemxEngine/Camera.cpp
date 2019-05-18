#include "stdafx.h"
#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>


/** 
*	@brief Constructor for camera
*
*	@author Demian Floreani
*
*/
Demx::Camera::Camera(void)
{
	xAngle = 0;
	yAngle = M_PI;
	dir = VEC3_Z;
}

Demx::Camera::~Camera(void)
{
}

/** 
*	@brief Set camera's position
*
*	@author Demian Floreani
*
*	@param[in] pos VEC3 specifying camera position
*	@param[in] up VEC3 specifying up vector
*
*   @return None
*/
GLvoid Demx::Camera::position( VEC3 pos, VEC3 up )
{
	this->pos	= pos;
	this->up	= up;
}

/** 
*	@brief Get camera's position
*
*	@author Demian Floreani
*
*   @return VEC3 The camera position
*/
VEC3 Demx::Camera::getPosition()
{
	return pos;
}

/** 
*	@brief Get camera's view vector (where it is looking at)
*
*	@author Demian Floreani
*
*
*   @return VEC3 The camera view vector
*/
VEC3 Demx::Camera::getView()
{
	return pos + dir;
}

/** 
*	@brief Get camera's up vector
*
*	@author Demian Floreani
*
*   @return VEC3 The camera up vector
*/
VEC3 Demx::Camera::getUp()
{
	return up;
}

/** 
*	@brief Change the camera's direction based on mouse input
*
*	@author Demian Floreani
*
*	@param[in] width Width of the screen in pixels
*	@param[in] height Height of the screen in pixels
*
*   @return None
*/
GLvoid Demx::Camera::calculateFromInput( GLuint width, GLuint height )
{
	GLint midX = width / 2;
	GLint midY = height / 2;

	POINT p;
	//get current mouse position
	GetCursorPos(&p);

	SetCursorPos(width / 2, height / 2);

	xAngle += 0.005f * GLfloat( p.x - midX );
	yAngle += 0.005f * GLfloat( p.y - midY );
	
	// set direction vector
	dir = glm::vec3 (
		 glm::sin(yAngle) * glm::cos(xAngle),
		 glm::cos(yAngle),
		 glm::sin(yAngle) * glm::sin(xAngle)
	);
}

/** 
*	@brief Move the camera forward based on direction vector
*
*	@author Demian Floreani
*
*	@param[in] speed The speed of movement
*
*   @return None
*/
GLvoid Demx::Camera::moveForward( GLfloat speed )
{
	pos.x += dir.x * speed;
	pos.y += dir.y * speed;
	pos.z += dir.z * speed;
}

/** 
*	@brief Move the camera backwards based on direction vector
*
*	@author Demian Floreani
*
*	@param[in] speed The speed of movement
*
*   @return None
*/
GLvoid Demx::Camera::moveBackward( GLfloat speed )
{
	pos.x += dir.x * speed;
	pos.y += dir.y * speed;
	pos.z += dir.z * speed;
}