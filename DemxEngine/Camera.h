#pragma once

class Renderer;

namespace Demx
{
	/** 
	*	@brief Class for using a camera in the scene
	*/
	class Camera
	{
		friend Renderer;

	private:
		VEC3 pos;		
		VEC3 up;	
		VEC3 dir;

		GLfloat xAngle;
		GLfloat yAngle;

	public:
		Camera( GLvoid );
		~Camera( GLvoid );
		
		GLvoid position( VEC3 pos, VEC3 up );

		GLvoid calculateFromInput( GLuint width, GLuint height );
		GLvoid moveForward( GLfloat speed = 0.1f );
		GLvoid moveBackward( GLfloat speed = 0.1f );

		VEC3 getPosition();
		VEC3 getView();
		VEC3 getUp();
	};
}