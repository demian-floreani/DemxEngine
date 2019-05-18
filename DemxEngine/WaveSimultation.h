#pragma once

class Renderer;

namespace Demx
{
	/** 
	*	@brief This class handles simulations of waves.
	*/
	class WaveSimultation : public Entity
	{
		friend class Renderer;

	private:
		GLfloat waveMovement;
		GLfloat modifier;
		GLfloat divisor;
		GLfloat speed;
		GLint size;

		GL_HRESULT init();

	public:
		WaveSimultation(GLint size, GLfloat speed = 50.0f, GLfloat modifier = 0.1f, GLfloat divisor = 4.0f);
		~WaveSimultation(GLvoid);

		GLvoid setSpeed( GLfloat speed );
		GLvoid setModifier( GLfloat modifier );
		GLvoid setDivisor( GLfloat divisor );

		GLvoid onStartDraw( Demx::Shader* program );
	};
}