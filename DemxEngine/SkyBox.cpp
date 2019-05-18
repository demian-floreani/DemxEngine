#include "stdafx.h"
#include "SkyBox.h"

/** 
*	@brief Constructor for the sky box
*
*	@author Demian Floreani
*
*	@param[in] directory The path where the sky box images are saved
*	@param[in] format The image format
*	@param[in] size Size of the skybox
*
*   @return None
*/
Demx::SkyBox::SkyBox(std::string directory, std::string format, GLfloat size) : Entity()
{
	this->directory = directory;
	this->format = format;
	this->size = size;
}

Demx::SkyBox::~SkyBox(void)
{
}

/** 
*	@brief Set the color of the skybox
*
*	@author Demian Floreani
*
*	@param[in] color Color of skybox
*
*   @return None
*/
void Demx::SkyBox::setColor( VEC4 color )
{
	this->color = color;
}

/** 
*	@brief Get the color of the skybox
*
*	@author Demian Floreani
*
*   @return VEC4 Color of skybox
*/
VEC4 Demx::SkyBox::getColor()
{
	return this->color;
}

/** 
*	@brief Loads the images located in the directory path in a cube map texture.
*
*	@author Demian Floreani
*
*   @return None
*/
void Demx::SkyBox::loadSkybox()
{
	cubeMapTexture = new Demx::Texture ( GL_TEXTURE_CUBE_MAP );

	cubeMapTexture->setupCubeMapTexture();

	cubeMapTexture->loadCubeMapTexture( directory + "ft" + "." + format, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z );
	cubeMapTexture->loadCubeMapTexture( directory + "bk" + "." + format, GL_TEXTURE_CUBE_MAP_POSITIVE_Z );
	cubeMapTexture->loadCubeMapTexture( directory + "up" + "." + format, GL_TEXTURE_CUBE_MAP_POSITIVE_Y );
	cubeMapTexture->loadCubeMapTexture( directory + "dn" + "." + format, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y );
	cubeMapTexture->loadCubeMapTexture( directory + "lf" + "." + format, GL_TEXTURE_CUBE_MAP_NEGATIVE_X );
	cubeMapTexture->loadCubeMapTexture( directory + "rt" + "." + format, GL_TEXTURE_CUBE_MAP_POSITIVE_X );

	cubeMapTexture->setFiltering(GL_LINEAR, GL_LINEAR);
	cubeMapTexture->setSamplerParameter( GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	cubeMapTexture->setSamplerParameter( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	cubeMapTexture->setSamplerParameter( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	float vertices[] = {
	  -size,  size, -size,
	  -size, -size, -size,
	   size, -size, -size,
	   size, -size, -size,
	   size,  size, -size,
	  -size,  size, -size,
  
	  -size, -size,  size,
	  -size, -size, -size,
	  -size,  size, -size,
	  -size,  size, -size,
	  -size,  size,  size,
	  -size, -size,  size,
  
	   size, -size, -size,
	   size, -size,  size,
	   size,  size,  size,
	   size,  size,  size,
	   size,  size, -size,
	   size, -size, -size,
   
	  -size, -size,  size,
	  -size,  size,  size,
	   size,  size,  size,
	   size,  size,  size,
	   size, -size,  size,
	  -size, -size,  size,
  
	  -size,  size, -size,
	   size,  size, -size,
	   size,  size,  size,
	   size,  size,  size,
	  -size,  size,  size,
	  -size,  size, -size,
  
	  -size, -size, -size,
	  -size, -size,  size,
	   size, -size, -size,
	   size, -size, -size,
	  -size, -size,  size,
	   size, -size,  size
	};

	GLuint vbo;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (GLfloat), &vertices, GL_STATIC_DRAW);

	glGenVertexArrays ( 1, &VAO );
	glBindVertexArray ( VAO );
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

/** 
*	@brief Release the skybox texture
*
*	@author Demian Floreani
*
*   @return None
*/
void Demx::SkyBox::releaseSkybox()
{
	delete cubeMapTexture;
}
