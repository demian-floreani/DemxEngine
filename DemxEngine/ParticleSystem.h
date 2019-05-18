#pragma once

#include "Entity.h"
#include "Shader.h"

namespace Demx
{
	typedef struct 
	{
		GLboolean	active;
		GLfloat		life;
		GLfloat		fade;
		GLfloat		speed;
		GLfloat		scale;
		GLfloat		fadeInTime;

		glm::vec3	origin;
		glm::vec4	color;
		glm::vec3	position;
		glm::vec3	direction;
		glm::vec3	gravity;
	} Particle;

	class ParticleSystem : public Entity
	{
		friend Renderer;

	private:
		std::vector<VEC4> instancePositions;
		std::vector<VEC4> instanceColors;

		std::vector<Particle> particles;

		void (*dynamics)(Particle&, GLfloat);

		GLuint			particleNum;
		GLboolean		loop;
		GLfloat			size;

		std::string		material;

		Particle		part;

		GLboolean		colorTransition;
		glm::vec3		colorStart;
		glm::vec3		colorEnd;

		GLboolean		fadeIn;
		GLfloat			fadeInTime;

		GLuint			vboPositions;
		GLuint			vboColors;

	public:
		ParticleSystem( GLuint particleNum );
		~ParticleSystem(void);

		void setParticleNum( GLuint num );
		void setPosition( glm::vec3 position );
		void setColor( glm::vec4 color );
		void setGravity( glm::vec3 gravity );
		void setSpeed( GLfloat speed );
		void setLoop( GLboolean loop );
		void setColorTransition( glm::vec3 start, glm::vec3 end, GLfloat step );
		void setDynamics( void (*fp)(Particle&, GLfloat) );
		void setSize( GLfloat size );
		void setFadeIn( GLfloat time );
		void setMaterial( std::string material );

	private:
		void init(GLvoid);
		void render();
		void update( GLfloat time );
	};
}