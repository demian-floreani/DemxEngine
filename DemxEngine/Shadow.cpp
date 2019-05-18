#include "stdafx.h"

#include "Shadow.h"

/** 
 *	@brief Constructor for the shadow.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] resolution The resolution in pixels of the shadow map texture. A higher value will give a more detailed shadow, but will impact performance.
 *
 *	@return None
 */
Demx::Shadow::Shadow( GLuint resolution )
{
	this->resolution = resolution;

	this->framebuffer = 0;
	this->depthTexture = 0;
	this->cubeMap = 0;

	this->depthIndex = -1;
	this->depthCubeIndex = -1;
}

/** 
 *	@brief Destructor for shadow.
 * 
 *	@author Demian Floreani
 *
 *	@return None
 */
Demx::Shadow::~Shadow(void)
{
	if ( framebuffer != 0 ) 
	{
        glDeleteFramebuffers(1, &framebuffer);
    }

    if ( depthTexture != 0 ) 
	{
        glDeleteTextures(1, &depthTexture);
    }	

	if ( cubeMap != 0) 
	{
        glDeleteTextures(1, &cubeMap);
    }	
}

/** 
 *	@brief Set the shadow resolution.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] resolution A value, in pixels, for the shadow map texture.

 *	@return None
 */
GLvoid Demx::Shadow::setResolution( GLuint resolution )
{
	this->resolution = resolution;
}
