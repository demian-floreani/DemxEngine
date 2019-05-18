#include "stdafx.h"
#include "Material.h"

/** 
 *	@brief Constructor for Material
 * 
 *	@author Demian Floreani
 *
 *	@return None
 */
Demx::Material::Material(void)
{
	specularColor = COLOR3_BLACK;
	phongExp = 1.0f;
	transparency = 1.0f;
}

/** 
 *	@brief Destructor for Material
 * 
 *	@author Demian Floreani
 *
 *	@return GL_HRESULT
 */
Demx::Material::~Material(void)
{
}

/** 
 *	@brief Set the specular color for the material
 * 
 *	@author Demian Floreani
 *
 *	@param[in] color COLOR3 that specifies a color

 *	@return None
 */
GLvoid Demx::Material::setSpecularColor( COLOR3 color )
{
	this->specularColor = color;
}

/** 
 *	@brief Set the phong exponent for the material
 * 
 *	@author Demian Floreani
 *
 *	@param[in] phongExp GLfloat that specifies the phong exponent for specular reflections. A higher value results in a more concentrated specular effect.

 *	@return None
 */
GLvoid Demx::Material::setPhongExp( GLfloat phongExp )
{
	if( phongExp != 0.0f )
	{
		this->phongExp = phongExp;
	}
}

/** 
 *	@brief Sets the transparency for the material
 * 
 *	@author Demian Floreani
 *
 *	@param[in] transparency The value of the transparency. Default value is 1.0f which represents an opaque material.

 *	@return None
 */
GLvoid Demx::Material::setTransparency( GLfloat transparency )
{
	if( transparency >= 0.0f && transparency <= 1.0f )
	{
		this->transparency = transparency;
	}
}

/** 
 *	@brief Add a texture to the material.
 * 
 *	@author Demian Floreani
 *
 *	@param[in] texture The name of the texture
 *  @param[in] contribution The amount the texture will contribute to the material. Default value is 1.

 *	@return None
 */
GLvoid Demx::Material::setTexture( const std::string& texture, float contribution )
{
	if( textures.size() < 3 )
	{
		std::pair<std::string, GLfloat> pair ( texture, contribution );
		textures.insert( pair );
	}
}

/** 
 *	@brief Get the specular color
 * 
 *	@author Demian Floreani
 *
 *	@return COLOR3
 */
const COLOR3& Demx::Material::getSpecularColor() const
{
	return specularColor;
}

/** 
 *	@brief Get the phong exponent
 * 
 *	@author Demian Floreani
 *
 *	@return GLfloat
 */
GLfloat Demx::Material::getPhongExp() const
{
	return phongExp;
}

/** 
 *	@brief Get the transparency value
 * 
 *	@author Demian Floreani
 *
 *	@return GLfloat
 */
GLfloat Demx::Material::getTransparency() const
{
	return transparency;
}

/** 
 *	@brief Find if the material is transparent.
 * 
 *	@author Demian Floreani
 *
 *	@return GLboolean
 */
GLboolean Demx::Material::isTransparent() const
{
	return transparency == 1.0f ? GL_FALSE : GL_TRUE;
}

/** 
 *	@brief Get the textures that form the material.
 * 
 *	@author Demian Floreani
 *
 *	@return unordered_map
 */
const std::unordered_map<std::string, GLfloat>& Demx::Material::getTextures() const
{
	return textures;
}