#pragma once

#include <unordered_map>

#include "Shader.h"

namespace Demx
{
	/** 
	*	@brief Class for managing the engine's shader programs
	* 
	*	@author Demian Floreani
	*/
	class ShaderManager
	{
	private:
		Shader* program;
		std::string current;

		std::unordered_map<std::string, Shader*> programs;
		
		GL_HRESULT loadShader(const std::string& fileName, GLuint shaderType, GLuint& shader);

	public:
		ShaderManager(GLvoid);
		~ShaderManager(GLvoid);

		GL_HRESULT addShaderProgram( const std::string& name, const std::string& vertex, const std::string& fragment); 
		Demx::Shader* useShaderProgram( const std::string& name );
		GL_HRESULT stopProgram();
		GL_HRESULT clean();
	};
}