#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene(void)
{
	addScript( "scripts\\scene2.txt" );
	addLuaScript( "scripts\\scene2.lua" );

	rot = 0.0f;
}


TestScene::~TestScene(void)
{
}

GL_HRESULT TestScene::Init( GLvoid )
{
	camera = new Demx::Camera();
	camera->position( VEC3( 0.0f, 4.0f, 4.0f ),  
					  VEC3_Y );

	this->setCamera( camera );

	sky = engine->addSkyBox("data\\skyboxes\\sky15\\", "jpg", 75.0f);

	engine->createAmbientLight( VEC3( 0.1f, 0.1f, 0.1f ) );

	//Demx::PointLight* pl1 = engine->createPointLight( COLOR3_WHITE, VEC3( 0.0f, 2.0f, 0.0f ) );
    //pl1->castShadow( GL_TRUE );

	//Demx::DirectionalLight* dl1 = engine->createDirectionalLight( new Demx::DirectionalLight( COLOR3_WHITE, -VEC3( 10.0f, 10.0f, 10.0f ) ) );
	//dl1->castShadow( GL_TRUE );
	//dl1->setShadowResolution( MEDIUM_RESOLUTION );
	//dl1->setShadowPosition( VEC3( 10.0f, 10.0f, 10.0f ) );

	//Demx::DirectionalLight* dl2 = engine->createDirectionalLight( new Demx::DirectionalLight( glm::vec3( 1.0f, 1.0f, 1.0f ), -VEC3( -10.0f, 10.0f, 5.0f ) ) );
	//dl2->castShadow( GL_TRUE );
	//dl2->setShadowPosition( VEC3( -10.0f, 10.0f, 5.0f ), -NORMALIZE( VEC3( -10.0f, 10.0f, 5.0f ) ) );

	Demx::SpotLight* sp2 = engine->createSpotLight( COLOR3_BLUE, VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ), 30.0f );
	sp2->castShadow( GL_TRUE );

	//Demx::Light* sp3 = engine->createSpotLight( COLOR3_RED, VEC3( 10.0f, 10.0f, -10.0f ), -VEC3( 10.0f, 10.0f, -10.0f ) );
	//sp3->castShadow( GL_TRUE );
	
	//Demx::SpotLight* sp4 = engine->createSpotLight( COLOR3_WHITE, VEC3( 10.0f, 10.0f, 10.0f ), -VEC3( 10.0f, 10.0f, 10.0f ) );
	//sp4->castShadow( GL_TRUE );
	//sp4->setShadowPosition( VEC3( 10.0f, 10.0f, 10.0f ), -VEC3( 10.0f, 10.0f, 10.0f ) );
	
	//Demx::SpotLight* sp4 = engine->createSpotLight( VEC3( 1.0f, 1.0f, 0.5f), VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ) );
	//sp4->castShadow( GL_TRUE );
	//sp4->setShadowPosition( VEC3( -10.0f, 10.0f, -10.0f ), -VEC3( -10.0f, 10.0f, -10.0f ) );

	//point = engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 0, 1.0, 0 ) );
	/*engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 1, 0 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, 10 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, -10 ) );
	engine->createPointLight( VEC3( 0.5, 0.5, 0.5 ), VEC3( 10, 2, -20 ) );*/

	//engine->createPointLight( COLOR3( 0.5f, 0.5f, 0.5f ), VEC3( -2.0f, 0.5f, 3.0f ) );

	engine->createEntity( "ground", Demx::Entity::createPlane( "metallic", 40.0f, 40, 40 ) );
	
	//Demx::GLEntity* pyramid = engine->createEntity( Demx::GLEntity::createPyramid( "metallic", 1, 1 ) );
	//pyramid->translate( 2.0f * VEC3_Y );

	//Demx::GLEntity* circle = engine->createEntity( Demx::GLEntity::createCylinder( "metallic", 1.0f, 2.0f, 1, 1 ) );
	//circle->translate( 2.0f * VEC3_Y );

	/*Demx::GLEntity* cone = engine->createEntity( Demx::GLEntity::createCone( "metallic", 2.0f, 4.0f, 1, 1 ) );
	cone->translate( 1.0f * VEC3_Y );*/

	return GL_OK;
}

GL_HRESULT TestScene::Loop( GLfloat timeDelta )
{

	return GL_OK;
}

GLvoid TestScene::Input( MESSAGE msg, PARAM param )
{
	switch( msg )
	{
	case KEYDOWN:							
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

