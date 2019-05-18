#include "stdafx.h"
#include "Shader.h"


Demx::Shader::Shader()
{
}

Demx::Shader::~Shader()
{
}

/** 
*	@brief Set the shader uniform names for the lights of the scene.
* 
*	@author Demian Floreani
*
*	@param[in] lights A pointer to the Lights struct containing lighting information
*
*	@return None
*/
GLvoid Demx::Shader::setPredefinedLights( const Demx::Lights* lights )
{
	setUniform( "numDirectionalLights", lights->directionalLights.size() );
	setUniform( "numPointLights",		lights->pointLights.size() );
	setUniform( "numSpotLights",		lights->spotLights.size() );

	std::string attrib;

	for( std::size_t i = 0; i < lights->directionalLights.size(); i++ )
	{
		std::stringstream ss;
		ss << i;

		std::vector<std::string> light;
		light.push_back("dirLights[" + ss.str() + "].color");
		light.push_back("dirLights[" + ss.str() + "].direction");
		light.push_back("dirLights[" + ss.str() + "].shadowIndex");

		predefinedDirectionalLights.push_back(light);
	}

	for( std::size_t i = 0; i < lights->pointLights.size(); i++ )
	{
		std::stringstream ss;
		ss << i;

		std::vector<std::string> light;
		light.push_back("pointLights[" + ss.str() + "].color");
		light.push_back("pointLights[" + ss.str() + "].position");
		light.push_back("pointLights[" + ss.str() + "].cAtt");
		light.push_back("pointLights[" + ss.str() + "].lAtt");
		light.push_back("pointLights[" + ss.str() + "].eAtt");
		light.push_back("pointLights[" + ss.str() + "].shadowIndex");

		predefinedPointLights.push_back(light);
	}

	for( std::size_t i = 0; i < lights->spotLights.size(); i++ )
	{
		std::stringstream ss;
		ss << i;

		std::vector<std::string> light;
		light.push_back("spotLights[" + ss.str() + "].color");
		light.push_back("spotLights[" + ss.str() + "].position");
		light.push_back("spotLights[" + ss.str() + "].direction");
		light.push_back("spotLights[" + ss.str() + "].coneCosine");
		light.push_back("spotLights[" + ss.str() + "].cAtt");
		light.push_back("spotLights[" + ss.str() + "].lAtt");
		light.push_back("spotLights[" + ss.str() + "].eAtt");
		light.push_back("spotLights[" + ss.str() + "].shadowIndex");

		predefinedSpotLights.push_back(light);
	}
}

/** 
*	@brief Set the values of the scene lighting uniforms.
* 
*	@author Demian Floreani
*
*	@param[in] lights A pointer to the Lights struct containing lighting information
*
*	@return None
*/
GLvoid Demx::Shader::setLights( const Lights* lights )
{
	if(lights->ambientLight != nullptr)
	{
		setUniform( "ambientLightActive", (GLuint) 1 );
		setUniform( "ambLight.color", lights->ambientLight->color );
	}

	for( std::size_t i = 0; i < lights->directionalLights.size(); i++ )
	{
		setUniform( predefinedDirectionalLights[i][0], lights->directionalLights[i]->isActive() ? lights->directionalLights[i]->color : VEC3_ZERO );
		setUniform( predefinedDirectionalLights[i][1], lights->directionalLights[i]->direction );
		setUniform( predefinedDirectionalLights[i][2], lights->directionalLights[i]->getShadowDepthIndex() );
	}

	for( std::size_t i = 0; i < lights->spotLights.size(); i++ )
	{	
		setUniform( predefinedSpotLights[i][0],	lights->spotLights[i]->isActive() ? lights->spotLights[i]->color : VEC3_ZERO );
		setUniform( predefinedSpotLights[i][1],	lights->spotLights[i]->position );
		setUniform( predefinedSpotLights[i][2],	lights->spotLights[i]->direction );
		setUniform( predefinedSpotLights[i][3],	lights->spotLights[i]->coneCosine );
		setUniform( predefinedSpotLights[i][4],	lights->spotLights[i]->constantAtt );
		setUniform( predefinedSpotLights[i][5],	lights->spotLights[i]->linearAtt );
		setUniform( predefinedSpotLights[i][6],	lights->spotLights[i]->expAtt );
		setUniform( predefinedSpotLights[i][7],	lights->spotLights[i]->getShadowDepthIndex() );
	}

 	for( std::size_t i = 0; i < lights->pointLights.size(); i++ )
	{	
		setUniform( predefinedPointLights[i][0], lights->pointLights[i]->isActive() ? lights->pointLights[i]->color : VEC3_ZERO );
		setUniform( predefinedPointLights[i][1], lights->pointLights[i]->position );
		setUniform( predefinedPointLights[i][2], lights->pointLights[i]->constantAtt );
		setUniform( predefinedPointLights[i][3], lights->pointLights[i]->linearAtt );
		setUniform( predefinedPointLights[i][4], lights->pointLights[i]->expAtt );
		setUniform( predefinedPointLights[i][5], lights->pointLights[i]->getShadowDepthIndex() );
	}
}

/** 
*	@brief Set the uniform of an unsigned int
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] ui Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint ui )
{
	glUniform1ui(	glGetUniformLocation(id, att.c_str()), 
					ui 
				);
}

/** 
*	@brief Set the uniform of an array of unsigned ints
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, GLuint* uiv )
{
	glUniform1uiv(	glGetUniformLocation(id, att.c_str()), 
					size,
					uiv
				);
}

/** 
*	@brief Set the uniform of a signed int
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] i Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLint i )
{	
	glUniform1i(	glGetUniformLocation(id, att.c_str()), 
					i 
				);
}

/** 
*	@brief Set the uniform of an array of signed ints
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, GLint* iv )
{
	glUniform1iv(	glGetUniformLocation(id, att.c_str()), 
					size,
					iv
				);
}

/** 
*	@brief Set the uniform of a float
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] ui Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLfloat f )
{
	glUniform1f(	glGetUniformLocation(id, att.c_str()), 
					f 
				);
}

/** 
*	@brief Set the uniform of an array of floats
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, GLfloat* fv )
{
	glUniform1fv(	glGetUniformLocation(id, att.c_str()), 
					size,
					fv
				);
}

/** 
*	@brief Set the uniform of a VEC3
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] ui Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, const VEC3& vec )
{
	glUniform3fv(	glGetUniformLocation(id, att.c_str()),	
					1,
					&vec[0]
				);
}

/** 
*	@brief Set the uniform of an array of VEC3
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, VEC3* vec3v )
{
	glUniform3fv(	glGetUniformLocation(id, att.c_str()),	
					size,
					(const float*)vec3v
				);
}

/** 
*	@brief Set the uniform of a VEC4
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] ui Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, const VEC4& vec )
{
	glUniform4fv(	glGetUniformLocation(id, att.c_str()), 
					1,
					&vec[0]
				);
}

/** 
*	@brief Set the uniform of an array of VEC4
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, VEC4* vec4v )
{
	glUniform4fv(	glGetUniformLocation(id, att.c_str()),	
					size,
					(const float*)vec4v
				);
}

/** 
*	@brief Set the uniform of a MAT4
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] ui Value
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, const MAT4& mat )
{
	glUniformMatrix4fv(		glGetUniformLocation(id, att.c_str()),
							1, 
							GL_FALSE, 
							&mat[0][0]
						);
}

/** 
*	@brief Set the uniform of an array of MAT4
* 
*	@author Demian Floreani
*
*	@param[in] att Name of the uniform
*	@param[in] size Size of the array
*	@param[in] ui The array values
*
*	@return None
*/
GLvoid Demx::Shader::setUniform( const std::string& att, GLuint size, MAT4* mat )
{
	glUniformMatrix4fv(		glGetUniformLocation(id, att.c_str()),
							size, 
							GL_FALSE, 
						    (const float*)mat
						);
}