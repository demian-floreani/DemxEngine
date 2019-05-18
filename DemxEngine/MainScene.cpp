#include "stdafx.h"
#include "MainScene.h"

void tree_trans( Demx::Entity* e );

MainScene::MainScene( GLvoid )
{
	angle = 45.0f;
	rot = 0.0f;
}


MainScene::~MainScene( GLvoid )
{
}

GL_HRESULT MainScene::Init( GLvoid )
{
	camera = new Demx::Camera();
	camera->position( VEC3( -13.0f, 3.5f, 14.0f ),  
					  VEC3_Y );
	this->setCamera( camera );


	engine->addTexture( "metal",	"data\\textures\\metal.jpg" );
	engine->addTexture( "grass",	"data\\textures\\grass.jpg" );
	engine->addTexture( "dry",		"data\\textures\\dry.jpg" );
	engine->addTexture( "particle", "data\\textures\\particle.bmp" );
	engine->addTexture( "smoke",    "data\\textures\\smoke.jpg" );
	engine->addTexture( "water",	"data\\textures\\water.jpg" );
	engine->addTexture( "sandgrass","data\\textures\\sandgrass.jpg" );

	engine->addModel( "house",		"data\\models\\house\\house.obj" );
	engine->addModel( "oak1",		"data\\models\\trees\\oak1.MD3" );
	engine->addModel( "oaka",		"data\\models\\trees\\oaka.MD3" );

	Demx::Material materials[2];

	

	materials[0].setSpecularColor( VEC3(1, 1, 1) );
	materials[0].setTexture( "metal" );

	materials[1].setTexture( "grass" );
	materials[1].setTexture( "dry" );

	
	sky = engine->addSkyBox(	"data\\skyboxes\\jajlands1\\", "jpg", 75.0f );
		
	
	//dl = engine->createDirectionalLight( new DirectionalLight( glm::vec3( 1.0f, 1.0f, 1.0f ), -NORMALIZE( VEC3( 10.0f, 10.0f, 5.0f ) ) ) );

	dl = engine->createDirectionalLight( VEC3( 1.0f, 1.0f, 1.0f ), -VEC3( 10.0f, 10.0f, 5.0f ) );

	//dl = engine->createDirectionalLight( new DirectionalLight( glm::vec3( 1.0f, 1.0f, 1.0f ), -NORMALIZE( VEC3( 10.0f, 10.0f, 5.0f ) ) ) );
	//dl = new DirectionalLight( glm::vec3( 1.0f, 0.0f, 1.0f ), -VEC3( 10.0f, 10.0f, 5.0f ) );
	
	//engine->createDirectionalLight( dl );
	
	dl->castShadow( true );
	//dl->shadow->setResolution( MEDIUM_RESOLUTION );
	
	
	/*

	sl = engine->createSpotLight( glm::vec3( .5f, .5f, .5f), glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3( 1.0f, 1.0f, 1.0f) );
	sl->setActive( GL_FALSE );

	engine->createPointLight( glm::vec3( 0.3f, 0.1f, 0.01f ), glm::vec3( -7.0f, 1.9f, 10.0f ) );


	Terrain* terrain = engine->addTerrain( 50.0f, 50.0f, 15, 15 );
	terrain->setTexture( "sandgrass" );
	terrain->setHeightMap( "data\\terrains\\terrain_1.hm" );
	

	Demx::GLEntity* house1 = engine->createEntity( "house" );

	house1->translate( VEC3( -2.0f, 1.5f, 8.0f ) );
	house1->scaleAll( 0.65f );
	house1->rotate( -20.0f, VEC3_Y );

	
	
	Demx::GLEntity* tree1 = engine->createEntity( "oaka" );
	tree1->translate( VEC3( 1.7f, 2.0f, 0.0f ) );
	tree_trans( tree1 );

	Demx::GLEntity* tree2 = engine->createEntity( "oak1" );
	tree2->translate( VEC3( 2.0f, 1.5f, -5.0f ) );
	tree_trans( tree2 );

	Demx::GLEntity* tree3 = engine->createEntity( "oak1" );
	tree3->translate( VEC3( -1.0f, 1.5f, -2.5f ) );
	tree_trans( tree3 );

	Demx::GLEntity* tree4 = engine->createEntity( "oak1" );
	tree4->translate( VEC3( 2.0f, 1.5f, -8.0f ) );
	tree_trans( tree4 );

	Demx::GLEntity* tree5 = engine->createEntity( "oak1" );
	tree5->translate( VEC3( -2.5f, 1.5f, -8.0f ) );
	tree_trans( tree5 );

	Demx::GLEntity* tree6 = engine->createEntity( "oak1" );
	tree6->translate( VEC3( -7.0f, 1.5f, 3.0f ) );
	tree_trans( tree6 );

	Demx::GLEntity* tree7 = engine->createEntity( "oak1" );
	tree7->translate( VEC3( -5.0f, 1.5f, -2.5f ) );
	tree_trans( tree7 );

	Demx::GLEntity* tree8 = engine->createEntity( "oak1" );
	tree8->translate( VEC3( 0.0f, 1.6f, 2.0f ) );
	tree_trans( tree8 );
	*/
	/*
	Demx::ParticleSystem* fire = engine->createParticleSystem( 250 );
//	fire->setTexture( "particle" );
	fire->setSize( 0.40f );
	fire->setSpeed( 1.5f );
	fire->setLoop( GL_TRUE );
	fire->setColorTransition( glm::vec3( 1.0f, 0.0f, 0.0f ), 
							  glm::vec3( 1.0f, 1.0f, 0.0f ), 8.0f );
	fire->setGravity( VEC3( 0.0f, 1.0f, 0.0f ) );
	fire->setPosition( glm::vec3( -7.0f, 1.7f, 10.0f ) );
	fire->setDynamics( []( Demx::Particle& p, GLfloat timeDelta ) -> void
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


	Demx::ParticleSystem* smoke = engine->createParticleSystem( 40 );
	smoke->setParticleNum( 40 );
//	smoke->setTexture( "smoke" );
	smoke->setSize( 0.25f );
	smoke->setSpeed( 0.35f );
	smoke->setLoop( GL_TRUE );
	smoke->setColor( VEC4( 0.225f, 0.19f, 0.11f, 1.0f ) );
	smoke->setGravity( VEC3( 0.0f, 1.0f, 0.0f ) );
	smoke->setPosition( glm::vec3( -7.0f, 4.2f, 10.0f ) );
	smoke->setFadeIn( 1000.0f );
	smoke->setDynamics( []( Demx::Particle& p, GLfloat timeDelta ) -> void
		{ 
			p.scale += 1.0f * timeDelta;

			if( p.position.y >= p.origin.y + 3.0f )
				p.fade += 0.01f * timeDelta;
		} );
	
	Demx::WaveSimultation* water = engine->addWaveSimultation( 20 );
	water->setTexture("water");
	water->translate( VEC3( -25.0f, 1.0f, -10.0f ) );
	*/
	
	return GL_OK;
}

GL_HRESULT MainScene::Loop( GLfloat timeDelta )
{
	/*sky->modelMatrix = MAT4( 1.0f );
	sky->modelMatrix = glm::rotate( sky->modelMatrix, rot, VEC3_Y );
	sky->color = VEC4( glm::sin( glm::radians( angle ) ), glm::sin( glm::radians( angle ) ), glm::sin( glm::radians( angle ) ), 1.0f );

	dl->direction = -glm::vec3( 20.0f * glm::cos( glm::radians( angle ) ), 
								 20.0f * glm::sin( glm::radians( angle ) ), 0.0f );
	
	dl->setShadowPosition( VEC3(	20.0f * glm::cos( glm::radians( angle ) ), 
									20.0f * glm::sin( glm::radians( angle ) ), 0.0f ), VEC3_ZERO );
	
	angle += 2.0f * timeDelta;
	if( angle > 360.0f )
		angle = 0.0f;

	if( sl->isActive() )
	{
		sl->setPosition( engine->getCamera()->getPosition() );
		sl->direction = glm::normalize( engine->getCamera()->getView() - engine->getCamera()->getPosition() );
	}

	rot += 2.0f * timeDelta;
	*/
	//cube->move( VEC3( 5.0f, 0.0f, 0.0f ), timeDelta, 1.0f );

	return GL_OK;
}

void MainScene::Input( MESSAGE msg, PARAM param )
{
	switch( msg )
	{
		case KEYDOWN:
		{
			switch ( param )
			{
				case 'F':
					sl->toggle();
				return;

				case 'W':
					camera->moveForward();
				return;

				case 'S':
					camera->moveBackward();
				return;
			};
		}break;
	}
	
}

void tree_trans( Demx::Entity* e )
{
	e->rotate( 90.0f, VEC3_Y );
	e->rotate( -90.0f, VEC3_X );
	e->scaleAll( 0.0075f );
}