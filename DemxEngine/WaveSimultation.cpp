#include "stdafx.h"

#include "Shader.h"
#include "Entity.h"

#include "WaveSimultation.h"

/** 
*	@brief Constructor for wave simulation
*
*	@author Demian Floreani
*
*	@param[in] size The sizein units of the wave simulation
*	@param[in] speed The speed of the waves
*	@param[in] modifier Changes the height of the waves
*	@param[in] divisor Changes the amplitude of the waves
*
*   @return None
*/
Demx::WaveSimultation::WaveSimultation(GLint size, GLfloat speed, GLfloat modifier, GLfloat divisor) : Entity( GL_TRIANGLE_STRIP )
{
	this->size = size;
	this->speed = speed;
	this->modifier = modifier;
	this->divisor = divisor;

	waveMovement = 0.0f;

	shader = "wave";
}

Demx::WaveSimultation::~WaveSimultation(GLvoid)
{
}

/** 
*	@brief Initializes the wave simulation by creating the vertices and texture mappings.
*
*	@author Demian Floreani
*
*   @return GL_HRESULT GL_OK if the initialization was succesful
*/
GL_HRESULT Demx::WaveSimultation::init()
{
	for ( int i = 0; i < size * 20; i++ )
	{
		position( VEC3(0.05f * GLfloat( i ), 0.0f, GLfloat(size) * -0.5f) );
		position( VEC3(0.05f * GLfloat( i ), 0.0f, GLfloat(size) * 0.5f) );
	}

	for ( int i = 0; i < size * 20; i++ )
	{
		map( VEC2(0.05f * GLfloat( i ), 0.0f) );
		map( VEC2(0.05f * GLfloat( i ), GLfloat(size) * 1.0f) );
	}

	return GL_OK;
}

/** 
*	@brief Set the speed of the wave simulation
*
*	@author Demian Floreani
*
*	@param[in] speed The speed of the waves
*
*   @return None
*/
GLvoid Demx::WaveSimultation::setSpeed( GLfloat speed )
{
	this->speed = speed;
}

/** 
*	@brief Set the height of the waves
*
*	@author Demian Floreani
*
*	@param[in] modifier Changes the height of the waves
*
*   @return None
*/
GLvoid Demx::WaveSimultation::setModifier( GLfloat modifier )
{
	this->modifier = modifier;
}

/** 
*	@brief Set the amplitude of the waves
*
*	@author Demian Floreani
*
*	@param[in] divisor Changes the amplitude of the waves
*
*   @return None
*/
GLvoid Demx::WaveSimultation::setDivisor( GLfloat divisor )
{
	this->divisor = divisor;
}

/** 
*	@brief Sets the operations to perform before rendering
*
*	@author Demian Floreani
*
*	@param[in] shader Pointer to the shader program that will be used for rendering
*
*   @return None
*/
GLvoid Demx::WaveSimultation::onStartDraw( Demx::Shader* program )
{
	program->setUniform("wave", waveMovement);
	program->setUniform("modifier", modifier);
	program->setUniform("divisor", divisor);
}