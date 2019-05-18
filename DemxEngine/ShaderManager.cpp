#include "stdafx.h"
#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "Shader.h"

#define BUFFER_SIZE		255

Demx::ShaderManager::ShaderManager(void)
{
	program = nullptr;
	current = "";
}

Demx::ShaderManager::~ShaderManager(void)
{
	clean();
}

/** 
*	@brief Load a shader program from a file.
* 
*	@author Demian Floreani
*
*	@param[in] fileName The path to the shader
*	@param[in] shaderType The type of shader.
*	@param[out] shader The id of the created shader
*
*	@return GL_HRESULT
*/
GL_HRESULT Demx::ShaderManager::loadShader( const std::string& fileName, GLuint shaderType, GLuint& shader)
{
	FILE* file;
	fopen_s( &file, fileName.c_str(), "rt");

	if(!file)
		return GL_FAIL;

	std::vector<std::string> lines;
	char line[ BUFFER_SIZE ];

	while( fgets( line, BUFFER_SIZE, file ) )
	{
		lines.push_back(line);
	}

	fclose(file);

	

	const char** source = new const char*[ lines.size() ];
	for( std::size_t i = 0; i < lines.size(); i++ )
	{
		source[i] = lines[i].c_str();
	}

	shader = glCreateShader( shaderType );
	glShaderSource( shader, lines.size(), source, NULL );
	glCompileShader( shader );

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	std::cout << "Compiling shader " << fileName << ".." << std::endl;
	if(status == GL_FALSE)
	{
		GLint maxLength = 100;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar* errorLog = new GLchar [ maxLength ];
		glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);

		std::cout << errorLog;

		delete[] errorLog;

		return GL_FAIL;
	}

	std::cout << "OK" << std::endl;

	return GL_OK;
}

/** 
*	@brief Adds a shader program.
* 
*	@author Demian Floreani
*
*	@param[in] name The name of the shader program
*	@param[in] vertex The path to the vertex shader file
*	@param[in] fragment The path to the fragment shader file
*
*	@return GL_HRESULT
*/
GL_HRESULT Demx::ShaderManager::addShaderProgram( const std::string& name, const std::string& vertex, const std::string& fragment )
{
	GLuint vertexShader, fragmentShader;

	loadShader(vertex, GL_VERTEX_SHADER, vertexShader);
	loadShader(fragment, GL_FRAGMENT_SHADER, fragmentShader);

	Shader* program = new Shader();
	program->id = glCreateProgram();
	program->fragmentShader = fragmentShader;
	program->vertexShader = vertexShader;

	glAttachShader(program->id, vertexShader);
	glAttachShader(program->id, fragmentShader);

	std::cout << "Linking " << name << ".." << std::endl;

	glLinkProgram(program->id);

	GLint linked;
	glGetProgramiv(program->id, GL_LINK_STATUS, &linked);

	if (linked)
	{
		std::cout << "OK" << std::endl;
		std::pair<std::string, Shader*> pair(name, program);
		this->programs.insert(pair);

		return GL_OK;
	}       

	std::cout << "FAIL" << std::endl;

	return GL_FAIL;
}

/** 
*	@brief Use a program that has been added to the shader manager.
* 
*	@author Demian Floreani
*
*	@param[in] name The name of the shader program
*
*	@return Shader* A pointer to the shader program
*/
Demx::Shader* Demx::ShaderManager::useShaderProgram( const std::string& name )
{
	if( current == name )
		return program;

	std::unordered_map<std::string, Shader*>::const_iterator got = programs.find(name);

	if ( !(got == programs.end()) )
	{
		current = name;
		program = got->second;
		glUseProgram(got->second->id);

		return program;
	}

	return nullptr;
}

/** 
*	@brief Stop using the shader program.
* 
*	@author Demian Floreani
*
*	@return GL_HRESULT
*/
GL_HRESULT Demx::ShaderManager::stopProgram()
{
	glUseProgram(0);
	return GL_OK;
}

/** 
*	@brief Release all the shader programs
* 
*	@author Demian Floreani
*
*	@return GL_HRESULT
*/
GL_HRESULT Demx::ShaderManager::clean()
{
	for(	std::unordered_map<std::string, Shader*>::iterator program = programs.begin(); 
			program != programs.end(); 
			++program)
    {
		glDeleteProgram(program->second->id);
		glDeleteProgram(program->second->vertexShader);
		glDeleteProgram(program->second->fragmentShader);
        delete program->second;
    }

	return GL_OK;
}