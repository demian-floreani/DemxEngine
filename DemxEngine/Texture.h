#pragma once

namespace Demx
{
	/** 
	*	@brief Class for loading textures.
	*/
	class Texture
	{
	private:
		GLenum targetTextureUnit;

		GLuint texture;
		GLuint sampler;

	public:
		std::string path;
		std::string name;

		Texture( GLenum targetTextureUnit = GL_TEXTURE_2D );
		~Texture();
		
		GLvoid bindTexture(GLenum textureUnit = 0) const;
		GLvoid setFiltering(GLint minFilter, GLint maxFilter);

		GL_HRESULT loadTexture( std::string path, GLboolean generateMipMaps = GL_TRUE );

		GL_HRESULT setupCubeMapTexture();
		GL_HRESULT loadCubeMapTexture( std::string path, GLenum targetTexture );

		GLvoid setSamplerParameter(GLenum parameter, GLenum value);
		GLvoid releaseTexture();
	};
}