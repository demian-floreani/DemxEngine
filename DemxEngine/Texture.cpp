#include "stdafx.h"
#include "texture.h"
#include <algorithm>

#include "FreeImage\Dist\x32\FreeImage.h"

/** 
 *	@brief Constructor for Texture class
 * 
 *	@author Demian Floreani
 *
 *	@param[in] targetTextureUnit Specify the target of the texture unit (GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP).
 *
 *	@return None
 */
Demx::Texture::Texture( GLenum targetTextureUnit )
{
	this->targetTextureUnit = targetTextureUnit;

	texture = 0;
	sampler = 0;
}

/** 
 *	@brief Destructor for Texture class
 * 
 *	@author Demian Floreani
 *
 *	@return None
 */
Demx::Texture::~Texture()
{
}

/** 
 *	@brief Loads a texture from a file path location
 * 
 *	@author Demian Floreani
 *
 *	@param[in] path The path to the texture image
 *  @param[in] generateMipMaps Specify if mip maps should be generated

 *	@return GL_HRESULT
 */
GL_HRESULT Demx::Texture::loadTexture( std::string path, GLboolean generateMipMaps )
{
	std::replace( path.begin(), path.end(), '\\', '/');

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib = nullptr;

	fif = FreeImage_GetFileType(path.c_str(), 0);

	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	}

	if(fif == FIF_UNKNOWN)
	{
		return GL_FAIL;
	}

	dib = FreeImage_Load(fif, path.c_str(), 0);

	if(!dib)
	{
		return GL_FAIL;
	}

	BYTE* dp = FreeImage_GetBits(dib);
	
	GLint width, height, bpp;

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	bpp = FreeImage_GetBPP(dib);

	if(dp == nullptr)
	{
		return GL_FAIL;
	}

	glGenTextures(1, &texture);
	glGenSamplers(1, &sampler);

	glBindTexture( this->targetTextureUnit, texture );
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, bpp == 24 ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, dp );

	if(generateMipMaps)
	{
		glGenerateMipmap( this->targetTextureUnit );
	}

	if(bpp == 24)
	{
		this->setFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	}
	else
	{
		this->setFiltering(GL_LINEAR, GL_LINEAR);
	}

	this->path = path;
	FreeImage_Unload(dib);

	return GL_OK;
}

/** 
 *	@brief Use this when creating a cube map texture.
 * 
 *	@author Demian Floreani
 *
 *	@return GL_HRESULT
 */
GL_HRESULT Demx::Texture::setupCubeMapTexture()
{
	glGenTextures( 1, &texture );
	glGenSamplers( 1, &sampler );

	return GL_OK;
}

/** 
 *	@brief Loads a texture from a file path location for a side of the texture cubemap.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] path The path to the texture image
 *  @param[in] targetTexture Specify the side of the cube map (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)

 *	@return GL_HRESULT
 */
GL_HRESULT Demx::Texture::loadCubeMapTexture( std::string path, GLenum targetTexture )
{
	std::replace( path.begin(), path.end(), '\\', '/');

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib = nullptr;

	fif = FreeImage_GetFileType(path.c_str(), 0);

	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	}

	if(fif == FIF_UNKNOWN)
	{
		return GL_FAIL;
	}

	dib = FreeImage_Load(fif, path.c_str(), 0);

	if(!dib)
	{
		return GL_FAIL;
	}

	BYTE* dp = FreeImage_GetBits(dib);
	
	int width, height, bpp;

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	bpp = FreeImage_GetBPP(dib);

	if(dp == nullptr)
	{
		return GL_FAIL;
	}

	glBindTexture( GL_TEXTURE_CUBE_MAP, texture );	
	glTexImage2D( targetTexture, 0, GL_RGBA8, width, height, 0, bpp == 24 ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, dp );

	FreeImage_Unload(dib);

	return GL_OK;
}

/** 
 *	@brief Set the filtering parameters for the texture.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] min_filter Value of GL_TEXTURE_MIN_FILTER
 *  @param[in] mag_filter Value of GL_TEXTURE_MAG_FILTER

 *	@return None
 */
GLvoid Demx::Texture::setFiltering( GLint min_filter, GLint mag_filter )
{
	glSamplerParameteri( sampler, GL_TEXTURE_MIN_FILTER, min_filter );
	glSamplerParameteri( sampler, GL_TEXTURE_MAG_FILTER, mag_filter );
}

/** 
 *	@brief Sets a generic parameter for the texture sampler
 * 
 *	@author Demian Floreani
 *
 *	@param[in] parameter Name of a sampler parameter
 *  @param[in] value Value of parameter

 *	@return None
 */
GLvoid Demx::Texture::setSamplerParameter( GLenum parameter, GLenum value )
{
	glSamplerParameteri( sampler, parameter, value );
}

/** 
 *	@brief Set the active texture to be used in the shaders
 * 
 *	@author Demian Floreani
 *
 *	@param[in] textureUnit Specify the texture unit to bound. Default value is 0.

 *	@return None
 */
GLvoid Demx::Texture::bindTexture( GLenum textureUnit ) const
{
	glActiveTexture( GL_TEXTURE0 + textureUnit );
	glBindTexture( this->targetTextureUnit, texture );
	glBindSampler( textureUnit, sampler );
}

/** 
 *	@brief Release the texture and sampler
 * 
 *	@author Demian Floreani
 *
 *	@return None
 */
GLvoid Demx::Texture::releaseTexture()
{
	glDeleteSamplers( 1, &sampler );
	glDeleteTextures( 1, &texture );
}