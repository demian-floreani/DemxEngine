#include "stdafx.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene(void)
{
	addScript( "scripts\\test.txt" );
	angle = 45.0f;
}


ParticleScene::~ParticleScene(void)
{
}

GL_HRESULT ParticleScene::Init( GLvoid )
{
	engine->setDebug( GL_TRUE );

	camera = new Demx::Camera();
	camera->position( VEC3( 4.0f, 6.0f, 12.0f ),  
					  VEC3_Y );
	this->setCamera( camera );

	sky = engine->addSkyBox("data\\skyboxes\\sky15\\", "jpg", 75.0f);

	//engine->createAmbientLight( VEC3(0.15f, 0.15f, 0.15f ) );

	/*Demx::DirectionalLight* dl1 = engine->createDirectionalLight( new Demx::DirectionalLight( 1.0f * COLOR3_WHITE, -VEC3( 10.0f, 10.0f, -10.0f ) ) );
	dl1->castShadow( GL_TRUE );
	dl1->setShadowResolution( MEDIUM_RESOLUTION );
	dl1->setShadowPosition( VEC3(10.0f, 10.0f, 0.0f) );
	*/

	//Demx::DirectionalLight* dl = engine->createDirectionalLight( new Demx::DirectionalLight( COLOR3_WHITE, -VEC3( 10.0f, 10.0f, -10.0f ) ) );
	//dl->setName( "dir1" );
	//dl->castShadow( GL_TRUE );
	//dl->setShadowResolution( HIGH_RESOLUTION );
	//dl->setShadowPosition( VEC3( 10.0f, 10.0f, -10.0f ) );


	//lamp post lights
	//engine->createPointLight( COLOR3_RED, VEC3(0, 7, 7.5) );
	//engine->createPointLight( COLOR3_BLUE, VEC3(-4, 7, -9) );

	//fire light
	Demx::PointLight* fireLight = engine->createPointLight( glm::vec3( 0.8f, 0.25f, 0 ), glm::vec3( -5, 6, 10 ) );
	fireLight->setName("fl");
	fireLight->setAttributes(0.2f, 0.02f, 0.0025f);

	engine->getParticleSystem( "fire" )
		->setDynamics( []( Demx::Particle& p, GLfloat timeDelta ) -> void
		{ 
			p.fade += 0.5f * timeDelta;

			if( p.position.x >= p.origin.x )
			{
				p.position.x -= 1.0f * timeDelta;
			}
			else
			{
				p.position.x += 1.0f * timeDelta;
			}

			if( p.position.z >= p.origin.z )
			{
				p.position.z -= 1.0f * timeDelta;
			}
			else
			{
				p.position.z += 1.0f * timeDelta;
			}

			if( p.position.y > p.origin.y + 1.5f )
				p.fade += 2.0f * timeDelta;
		} );

	engine->getParticleSystem( "smoke" )
		->setDynamics( []( Demx::Particle& p, GLfloat timeDelta ) -> void
		{ 
			p.scale += 1.0f * timeDelta;

			if( p.position.y >= p.origin.y + 3.0f )
				p.fade += 0.01f * timeDelta;
		} );

	return GL_OK;
}

GL_HRESULT ParticleScene::Loop( GLfloat timeDelta )
{
	VEC4 color =	VEC4(	glm::sin( glm::radians( angle ) ), 
							glm::sin( glm::radians( angle ) ), 
							glm::sin( glm::radians( angle ) ), 
							1.0f );
	color = glm::max( color, VEC4_ZERO ) + VEC4(0.1, 0.1, 0.1, 0);
	sky->setColor( color );

	VEC3 dir = VEC3(	20.0f * glm::cos( glm::radians( angle ) ), 
						20.0f * glm::sin( glm::radians( angle ) ), 
						20.0f * glm::cos( glm::radians( angle ) ) );

	Demx::DirectionalLight* l = engine->getDirectionalLight( "dl1" );
	l->direction = -dir;
	l->setShadowPosition( dir, VEC3_Y );
	
	angle += 8.0f * timeDelta;
	if( angle > 360.0f )
		angle = 0.0f;

	Demx::PointLight* pl = engine->getPointLight("fl");

	if(pl->color.r > 0.75)
		pl->color.r -= ((rand() % 50) / 100.f) * 1.0f * timeDelta;
	else
		pl->color.r -= ((rand() % 100 - 50) / 100.f) * 1.0f * timeDelta;

	if(pl->color.g < 0.3)
		pl->color.g += ((rand() % 100 - 50) / 100.f) * 1.0f * timeDelta;
	else
		pl->color.g -= ((rand() % 50) / 100.f) * 1.0f * timeDelta;
	if( pl->color.g < 0 )
		pl->color.g = 0;

	//std::cout << pl->color.r << " " << pl->color.g << " " << pl->color.b << " " << std::endl;

	//if(sl != nullptr){
	//	sl->setPosition(camera->getPosition());
	//	sl->setDirection(camera->getView());
	//}

	return GL_OK;
}

GLvoid ParticleScene::Input( MESSAGE msg, PARAM param )
{
	switch( msg )
	{
		case WM_KEYDOWN:
		{
			switch ( param )
			{
				case 'F':
					//sl->toggle();
				break;

				case 'W':
					camera->moveForward();
				break;

				case 'S':
					camera->moveBackward();
				break;

				case 'L':
				{
					engine->getPointLight("pl1")->toggle();
					engine->getPointLight("pl2")->toggle();
				}
			};
		}
	}
}
