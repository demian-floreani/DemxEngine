#pragma once

#include "Entity.h"

namespace Demx
{
	/** 
	*	@brief Class loading terrains.
	*/
	class Terrain : public Demx::Entity
	{
	public:
		GLfloat sizeX;
		GLfloat sizeZ; 

		GLuint gridX;
		GLuint gridZ;
	
		std::string heightMap;

		Terrain( std::string path );
		~Terrain();

		GL_HRESULT init();
		void setHeightMap( std::string path );
		GLvoid Terrain::onStartDraw( Demx::Shader* program ) override;
		GLvoid Terrain::onEndDraw() override;
	};
}
