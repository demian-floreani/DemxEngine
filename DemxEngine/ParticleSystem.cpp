#include "stdafx.h"
#include "ParticleSystem.h"
#include <time.h>

Demx::ParticleSystem::ParticleSystem( GLuint particleNum ) : Entity()
{
	this->setParticleNum( particleNum );
	setSize( 0.2f );
	colorTransition = GL_FALSE;
	fadeIn = GL_FALSE;
	loop = GL_TRUE;
	dynamics = nullptr;
	setPosition( VEC3_ZERO );
}

Demx::ParticleSystem::~ParticleSystem(GLvoid)
{
}


void Demx::ParticleSystem::init(GLvoid)
{
	position( VEC3( -size, -size, 0 ) );
	position( VEC3( size, -size, 0 ) );
	position( VEC3( -size, size, 0 ) );
	position( VEC3( size, size, 0 ) );

	map( VEC2( 0, 0 ) );
	map( VEC2( 1, 0 ) );
	map( VEC2( 0, 1 ) );
	map( VEC2( 1, 1 ) );

	instancePositions.reserve( this->particleNum );
	instanceColors.reserve( this->particleNum );

	for ( GLuint i=0; i < particleNum; ++i )                 
	{
		Particle p;

		p.active = GL_TRUE;
		p.life = 1.0f;               
		p.fade = GLfloat( rand() % 100 + 1 ) / 100.0f + 0.1f;   
		p.speed = part.speed;
		p.origin = part.position;
		p.position = part.position;
		p.direction.x = GLfloat( (rand() % 150) - 75 );  
		p.direction.y = GLfloat( (rand() % 150) - 75 );  
		p.direction.z = GLfloat( (rand() % 150) - 75 );     
		p.color = part.color;
		p.gravity = part.gravity;
		p.scale = 1.0f;
		p.fadeInTime = 0.0f;

		if( fadeIn == GL_TRUE )
		{
			p.life = 0.0f;
			p.fadeInTime = this->fadeInTime;
		}

		particles.push_back( p );

		instancePositions.push_back(VEC4());
		instanceColors.push_back(VEC4());
	}

	update( 0.5f );

	setupAABB( instancePositions );
}


void Demx::ParticleSystem::render()
{
	glBindVertexArray( VAO );

	glBindBuffer(GL_ARRAY_BUFFER, this->vboPositions);
	glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleNum * sizeof(GLfloat) * 4, &this->instancePositions[0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->vboColors);
	glBufferData(GL_ARRAY_BUFFER, particleNum * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleNum * sizeof(GLfloat) * 4, &this->instanceColors[0]);
	
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleNum);
}


void Demx::ParticleSystem::update( GLfloat time )
{
	GLuint i = 0;
	for (	std::vector<Particle>::iterator particle = particles.begin();
			particle != particles.end();
			++particle
		)                   
	{
		if( (*particle).active == GL_TRUE )
		{
			if( (*particle).direction != VEC3_ZERO )
			{
				(*particle).position += ( NORMALIZE( (*particle).direction ) * (*particle).speed * time );
			}

			(*particle).position += (*particle).gravity * (*particle).speed * time;

			if( fadeIn == GL_TRUE )
			{
				if( (*particle).fadeInTime > 0.0f )
				{
					(*particle).fadeInTime -= ( time * 1000.0f );
					(*particle).life += ( time * 1000.0f ) / this->fadeInTime;
				}
				else
					(*particle).life -= (*particle).fade * time;
			}
			else
			{
				(*particle).life -= (*particle).fade * time;      
			}

			(*particle).color.a = (*particle).life;
	
			if( dynamics != nullptr )
			{
				dynamics( (*particle), time );
			}

			if( colorTransition == GL_TRUE )
			{
				(*particle).color.r += ( colorStart.r * time );
				(*particle).color.g += ( colorStart.g * time );
				(*particle).color.b += ( colorStart.b * time );

				if( (*particle).color.r > colorEnd.r )
					(*particle).color.r = colorEnd.r;
				if( (*particle).color.g > colorEnd.g )
					(*particle).color.g = colorEnd.g;
				if( (*particle).color.b > colorEnd.b )
					(*particle).color.b = colorEnd.b;
			}

			//update instance position
			instancePositions[i].x = (*particle).position.x;
			instancePositions[i].y = (*particle).position.y;
			instancePositions[i].z = (*particle).position.z;
			instancePositions[i].w = (*particle).scale;

			//update instance color
			instanceColors[i].r = (*particle).color.r;
			instanceColors[i].g = (*particle).color.g;
			instanceColors[i].b = (*particle).color.b;
			instanceColors[i].a = (*particle).color.a;

			if( (*particle).life <= 0.0f && (*particle).fadeInTime <= 0.0f )
			{
				(*particle).active = GL_FALSE;
				
				if( loop == GL_TRUE )
				{
					(*particle).active = GL_TRUE;
					(*particle).life = 1.0f;               
					(*particle).fade = GLfloat( rand() % 100 + 1 ) / 100.0f + 0.1f;   
					(*particle).speed = part.speed;
					(*particle).origin = part.position;
					(*particle).position = part.position;
					(*particle).direction.x = GLfloat( (rand() % 150) - 75 );  
					(*particle).direction.y = GLfloat( (rand() % 150) - 75 );  
					(*particle).direction.z = GLfloat( (rand() % 150) - 75 );     
					(*particle).color = part.color;
					(*particle).gravity = part.gravity;
					(*particle).scale = 1.0f;
					(*particle).fadeInTime = 0.0f;

					if( fadeIn == GL_TRUE )
					{
						(*particle).life = 0.0f;
						(*particle).fadeInTime = this->fadeInTime;
					}
				}
			}
		}

		++i;
	}
}


void Demx::ParticleSystem::setMaterial( std::string material )
{
	this->material = material;
}

void Demx::ParticleSystem::setParticleNum( GLuint num )
{
	this->particleNum = num;
}

void Demx::ParticleSystem::setPosition( glm::vec3 position )
{
	this->part.position = position;
}

void Demx::ParticleSystem::setColor( glm::vec4 color )
{
	this->part.color = color;
}

void Demx::ParticleSystem::setGravity( glm::vec3 gravity )
{
	this->part.gravity = gravity;
}

void Demx::ParticleSystem::setSpeed( GLfloat speed )
{
	this->part.speed = speed;
}

void Demx::ParticleSystem::setLoop( GLboolean loop )
{
	this->loop = loop;
}

void Demx::ParticleSystem::setColorTransition( glm::vec3 start, glm::vec3 end, GLfloat step )
{
	part.color = glm::vec4( start, 1.0f );

	this->colorStart.r = ( ( end.r - start.r ) / step );
	this->colorStart.g = ( ( end.g - start.g ) / step  );
	this->colorStart.b = ( ( end.b - start.b ) / step  );
	//this->colori.a = 0.0f;
	colorEnd = end;

	colorTransition = true;
}

void Demx::ParticleSystem::setDynamics( void (*fp)(Particle&, GLfloat) )
{
	this->dynamics = fp;
}

void Demx::ParticleSystem::setSize( GLfloat size )
{
	this->size = size;
}

void Demx::ParticleSystem::setFadeIn( GLfloat time )
{
	fadeIn = GL_TRUE;
	fadeInTime = time;
}
