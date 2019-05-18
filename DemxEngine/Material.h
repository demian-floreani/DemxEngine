#pragma once

#include "texture.h"

#include <unordered_map>

namespace Demx
{
	/** 
	*	@brief Stores material information
	* 
	*	@author Demian Floreani
	*/
	class Material
	{
	private:
		COLOR3 specularColor;
		GLfloat phongExp;
		
		GLfloat transparency;
		std::unordered_map<std::string, GLfloat> textures;
	public:
		Material();
		~Material();

		GLvoid setSpecularColor( COLOR3 color );
		GLvoid setPhongExp( GLfloat phongExp );
		GLvoid setTransparency( GLfloat transparency );
	
		GLvoid setTexture( const std::string& texture, GLfloat contribution = 1.0f );

		const COLOR3& getSpecularColor() const;
		GLfloat getPhongExp() const;
		GLfloat getTransparency() const;
		GLboolean isTransparent() const;
		const std::unordered_map<std::string, GLfloat>& getTextures() const;
	};
}