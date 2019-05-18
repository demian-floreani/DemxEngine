#include "stdafx.h"

#include "Renderer.h"
#include "Engine.h"

/** 
 *	@brief The update method will provide operations that must be repeated in between the rendering of each frame.
 *  This includes updating the entities AABB and updating the view matrix based on camera.
 * 
 *	@author Demian Floreani
 *
 *	@return None
 */
GLvoid Demx::Renderer::update()
{
	if( lastTime == 0 )
	{
		lastTime = GetTickCount();
		timePassed = 0.0f;
	}
	else
	{
		DWORD currentTime = GetTickCount();
		timePassed = GLfloat(currentTime - lastTime);
		lastTime = currentTime;
		timePassed /= 1000.0f;
	}
	
	timeElapsed += timePassed;

	if(timeElapsed >= 1.0f)
	{
		//std::cout << frames << std::endl;
		frames = 0;
		timeElapsed = 0.0f;
	}
	++frames;
	

	cam->calculateFromInput( width, height );

	// calculate projection view matrix
	viewMatrix = glm::lookAt( cam->getPosition(), cam->getView(), cam->getUp() );
	projViewMatrix = projectionMatrix * viewMatrix;

	// set camera frustum
	camFrustum->set( projViewMatrix );

	for(	std::vector<Demx::Entity*>::const_iterator entity = pEngine->entities.begin(); 
			entity != pEngine->entities.end(); 
			++entity )
	{
		// update AABB with model matrix
		(*entity)->box.transform( (*entity)->modelMatrix );
	}

	for(	std::vector<Demx::WaveSimultation*>::const_iterator entity = pEngine->fluidSimultations.begin(); 
			entity != pEngine->fluidSimultations.end(); 
			++entity )
	{
		// update AABB with model matrix
		(*entity)->box.transform( (*entity)->modelMatrix );

		(*entity)->waveMovement > 360.0f ?              
			(*entity)->waveMovement = 0.0f :
			(*entity)->waveMovement += ( (*entity)->speed * timePassed );
	}
}
