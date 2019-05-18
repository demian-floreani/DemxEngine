#include "stdafx.h"
#include "Scene2.h"


Scene2::Scene2(void)
{
	addScript( "scripts\\scene2.txt" );
	//addLuaScript( "scripts\\scene2.lua" );

	rot = 0.0f;
	i = 0;
}


Scene2::~Scene2(void)
{
}

GL_HRESULT Scene2::Init( GLvoid )
{
	//engine->setDrawNormals( TRUE );

	camera = new Demx::Camera();
	camera->position( VEC3( 0.0f, 4.0f, 4.0f ),  
					  VEC3_Y );

	this->setCamera( camera );

	sky = engine->addSkyBox("data\\skyboxes\\sky15\\", "jpg", 75.0f);

	engine->createAmbientLight( VEC3( 0.1f, 0.1f, 0.1f ) );

	Demx::Material test;
	test.setTexture( "metal" );
	test.setTransparency( 0.3f );

	engine->addMaterial( "transparent_metal", test );

	engine->createEntity( "ground", Demx::Entity::createPlane( "ground", 100, 100, 100 ) );

	engine->createEntity( "cube", Demx::Entity::createCube( "transparent_metal", 1, 1 ) )
		->translate( VEC3(0.0f, 2.0f, 5.0f) )
		;

	engine->createEntity( "circle", Demx::Entity::createCircle( "transparent_metal", 2, 1, 1 ) )
		->translate( VEC3(2.0f, 2.0f, 0) )
		;

	engine->createEntity( "pyramid", Demx::Entity::createPyramid( "transparent_metal", 1, 1 ) )
		->translate( VEC3(2.0f, 2.0f, -5.0f) )
		;

	engine->createEntity( "o_1", Demx::Entity::createCube( "marble_mat", 1, 1 ) )
		->translate( VEC3(5.0f, 2.0f, 2.0f) )
		;

	engine->createEntity( "o_2", Demx::Entity::createCube( "marble_mat", 1, 1 ) )
		->translate( VEC3(-5.0f, 2.0f, 3.0f) )
		;
	
	//Demx::PointLight* pl1 = engine->createPointLight( COLOR3_WHITE, VEC3( 0.0f, 3.0f, 0.0f ) );
    //pl1->castShadow( GL_TRUE );
	//pl1->setShadowResolution( LOW_RESOLUTION );


	/*Demx::GLEntity* lightCube = engine->createEntity( Demx::GLEntity::createCube( "metallic", 1, 1 ) );
	lightCube->translate( VEC3( 0.0f, 4.0f, 0.0f ) );
	lightCube->scaleAll( 0.1f );*/

	//Demx::GLEntity* cube1 = engine->createEntity( Demx::GLEntity::createCube( "metallic", 1, 1 ) );

	///cube1->translate( VEC3( 0, 2, 0 ) );
	//cube1->scaleAll( 0.5f );

	/*Demx::GLEntity* cube2 = engine->createEntity( Demx::GLEntity::createCube( "metallic", 1, 1 ) );
	cube2->translate( VEC3( -0.9f, 1.5f, 0.0f ) );
	cube2->scaleAll( 0.5f );*/
	

	/*Demx::GLEntity* wall = new Demx::GLEntity(GL_TRIANGLES);
	wall->triangle( VEC3( 4.0f, 8.0f, -5.0f ), VEC3( 4.0f, 0.0f, -5.0f ), VEC3( 4.0f, 8.0f, 5.0f ) );
	wall->triangle( VEC3( 4.0f, 8.0f, 5.0f ), VEC3( 4.0f, 0.0f, -5.0f ), VEC3( 4.0f, 0.0f, 5.0f ) );
	wall->map( VEC2( 0, 1 ) );
	wall->map( VEC2( 0, 0 ) );
	wall->map( VEC2( 1, 1 ) );
	wall->map( VEC2( 1, 1 ) );
	wall->map( VEC2( 0, 0 ) );
	wall->map( VEC2( 1, 0 ) );
	wall->setMaterial( "metallic" );
	engine->createEntity( wall );*/

	Demx::DirectionalLight* dl1 = engine->createDirectionalLight( new Demx::DirectionalLight( COLOR3_WHITE, -VEC3( 10.0f, 5.0f, 5.0f ) ) );
//	dl1->castShadow( GL_TRUE );
//	dl1->setShadowResolution( MEDIUM_RESOLUTION );
	//dl1->setShadowPosition( VEC3( 10.0f, 5.0f, 5.0f ), -NORMALIZE( VEC3( 10.0f, 5.0f, 5.0f ) ) );

	//Demx::DirectionalLight* dl2 = engine->createDirectionalLight( new Demx::DirectionalLight( glm::vec3( 1.0f, 1.0f, 1.0f ), -VEC3( -10.0f, 10.0f, 5.0f ) ) );
	//dl2->castShadow( GL_TRUE );
	//dl2->setShadowPosition( VEC3( -10.0f, 10.0f, 5.0f ), -NORMALIZE( VEC3( -10.0f, 10.0f, 5.0f ) ) );

	//Demx::Light* sp1 = engine->createSpotLight( 0.5f * COLOR3_RED, VEC3( 0.0f, 8.0f, 0.0f ), -VEC3_Y, 45.0f );
	//sp1->castShadow( GL_TRUE );
	//sp1->setShadowPosition( VEC3( 0.0f, 8.0f, 0.0f ), -VEC3_Y, VEC3_Z );

	//Demx::SpotLight* sp2 = engine->createSpotLight( COLOR3_WHITE, VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ), 30.0f );
	//sp2->castShadow( GL_TRUE );
	//sp2->setShadowPosition( VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ) );
	
	//Demx::Light* sp3 = engine->createSpotLight( COLOR3_RED, VEC3( 10.0f, 10.0f, -10.0f ), -VEC3( 10.0f, 10.0f, -10.0f ) );
	//sp3->castShadow( GL_TRUE );
	//sp3->setShadowPosition( VEC3( 10.0f, 10.0f, -10.0f ), -VEC3( 10.0f, 10.0f, -10.0f ) );

	/*Demx::Light* sp4 = engine->createSpotLight( COLOR3_PURPLE, VEC3( 10.0f, 10.0f, 10.0f ), -VEC3( 10.0f, 10.0f, 10.0f ) );
	sp4->castShadow( GL_TRUE );
	sp4->setShadowPosition( VEC3( 10.0f, 10.0f, 10.0f ), -VEC3( 10.0f, 10.0f, 10.0f ) );*/

	//Demx::SpotLight* sp4 = engine->createSpotLight( VEC3( 1.0f, 1.0f, 0.5f), VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ) );
	//sp4->castShadow( GL_TRUE );
	//sp4->setShadowPosition( VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ) );

	//point = engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 0, 1.0, 0 ) );
	/*engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 1, 0 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, 10 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, -10 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, -20 ) );*/

	//engine->createPointLight( COLOR3( 0.5f, 0.5f, 0.5f ), VEC3( -2.0f, 0.5f, 3.0f ) );

	//engine->createEntity( Demx::GLEntity::createPlane( "ground", 10.0f, 10, 10 ) );
	
	//Demx::GLEntity* pyramid = engine->createEntity( Demx::GLEntity::createPyramid( "metallic", 1, 1 ) );
	//pyramid->translate( 2.0f * VEC3_Y );

	//Demx::GLEntity* circle = engine->createEntity( Demx::GLEntity::createCylinder( "metallic", 1.0f, 2.0f, 1, 1 ) );
	//circle->translate( 2.0f * VEC3_Y );

	/*Demx::GLEntity* cone = engine->createEntity( Demx::GLEntity::createCone( "metallic", 2.0f, 4.0f, 1, 1 ) );
	cone->translate( 1.0f * VEC3_Y );*/

	//cube1 = engine->createEntity( Demx::GLEntity::createCube( "metallic", 1, 1 ) );
	//cube1->setPosition( VEC3( 5.0f, 1.0f, 0.0f ) );
	//cube1->rotate( 45.0f, VEC3_Y );

	



	//Terrain* terrain = engine->addTerrain( "data\\terrains\\terrain_2.hm", "ground" );
	
	
/*	Demx::GLEntity* oil = engine->createEntity( "oil" );
	oil->translate( VEC3( 5.0f, 0.0f, 0.0f ) );
	oil->scaleAll( 2.0f );*/
	

	/*Demx::ParticleSystem* fire = engine->createParticleSystem( 250 );
	fire->setMaterial( "particle" );
	fire->setSize( 0.30f );
	fire->setSpeed( 1.5f );
	fire->setLoop( GL_TRUE );
	fire->setColorTransition( glm::vec3( 1.0f, 0.0f, 0.0f ), 
							  glm::vec3( 1.0f, 1.0f, 0.0f ), 8.0f );
	fire->setGravity( VEC3( 0.0f, 1.0f, 0.0f ) );
	fire->setPosition( glm::vec3( 5.0f, 3, 5.0f ) );
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
		} );*/
	
	//Demx::ParticleSystem* smoke = engine->createParticleSystem( 20 );
	
	/*smoke->setMaterial( "smoke" );
	smoke->setSize( 0.25f );
	smoke->setSpeed( 0.25f );
	smoke->setLoop( GL_TRUE );
	smoke->setColor( VEC4( 0.225f, 0.19f, 0.11f, 1.0f ) );
	smoke->setGravity( VEC3( 0.0f, 1.0f, 0.0f ) );
	smoke->setPosition( glm::vec3( 0.0f, 3, 0.0f ) );
	smoke->setFadeIn( 2000.0f );
	smoke->setDynamics( []( Demx::Particle& p, GLfloat timeDelta ) -> void
		{ 
			p.scale += 1.0f * timeDelta;

			if( p.position.y >= p.origin.y + 3.0f )
				p.fade += 0.01f * timeDelta;
		} );
		*/

	Demx::WaveSimultation* flag = engine->addWaveSimultation( 1 );
	flag->setMaterial( "flag_mat" );

	flag->setModifier( 0.05f );
	flag->setDivisor( 1.0f );
	flag->setSpeed( 100.0f );

	flag->translate(VEC3(0, 3, 0));
	flag->rotate(-90, VEC3_Z);
	flag->rotate(90, VEC3_Y);

	return GL_OK;
}

GL_HRESULT Scene2::Loop( GLfloat timeDelta )
{
	//cube1->resetModelMatrix();

	//cube1->translate( VEC3_Y );
	//cube1->rotate( rot, VEC3_Y );
	
	//cube1->move( VEC3( 5.0f, 1.0f, 0.0f ), timeDelta, 1.0f );

	//rot += 5.0f * timeDelta;
	//cube1->rotate(rot, VEC3_Y);*/

	return GL_OK;
}

GLvoid Scene2::Input( MESSAGE msg, PARAM param )
{
	switch( msg )
	{
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			switch ( param )
			{
				case 'W':
					camera->moveForward();
				break;

				case 'S':
					camera->moveBackward();
				break;

				case 'L':
//					point->toggle();
				break;
			};
		}
	}
}

