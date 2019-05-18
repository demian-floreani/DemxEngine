#pragma once
#include "Lights.h"

namespace Demx
{
	/** 
	*	@brief Manages a shader program
	* 
	*	@author Demian Floreani
	*/
	class Shader
	{
		std::vector<std::vector<std::string>> predefinedDirectionalLights;
		std::vector<std::vector<std::string>> predefinedPointLights;
		std::vector<std::vector<std::string>> predefinedSpotLights;

	public:
		Shader();
		~Shader();

		GLuint id;
		GLuint vertexShader;
		GLuint fragmentShader;

		GLvoid setUniform( const std::string& att, GLuint ui );
		GLvoid setUniform( const std::string& att, GLuint size, GLuint* uiv );

		GLvoid setUniform( const std::string& att, GLint i );
		GLvoid setUniform( const std::string& att, GLuint size, GLint* iv );

		GLvoid setUniform( const std::string& att, GLfloat f );
		GLvoid setUniform( const std::string& att, GLuint size, GLfloat* fv );

		GLvoid setUniform( const std::string& att, const VEC3& vec );
		GLvoid setUniform( const std::string& att, GLuint size, VEC3* vec3v );

		GLvoid setUniform( const std::string& att, const VEC4& vec4 );
		GLvoid setUniform( const std::string& att, GLuint size, VEC4* vec4v );

		GLvoid setUniform( const std::string& att, const MAT4& mat );
		GLvoid setUniform( const std::string& att, GLuint size, MAT4* mat );

		GLvoid setPredefinedLights( const Demx::Lights* lights );
		GLvoid setLights( const Demx::Lights* lights );
	};
}