#pragma once
#include "texture.h"
#include "Entity.h"

class Renderer;

namespace Demx
{
	/** 
	*	@brief Class for managing the scene skybox. To load correctly a directory with 6 textures is needed. The names of the 
	*   textures must follow the following pattern where <format> is the image's format:
	*   - ft.<format>
	*   - bk.<format>
	*   - up.<format>
	*   - dn.<format>
	*   - lf.<format>
	*   - rt.<format>
	*/
	class SkyBox : public Entity
	{
	friend Renderer;

	private:
		float* vertices;
		std::string directory;
		std::string format;
		VEC4 color;
		GLfloat size;
		Demx::Texture* cubeMapTexture;

	public:
		SkyBox(std::string directory, std::string format, GLfloat size);
		~SkyBox();

		void setColor( VEC4 color );
		VEC4 getColor();

	private:
		void loadSkybox();
		void releaseSkybox();
	};
}