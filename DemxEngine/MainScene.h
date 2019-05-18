#pragma once
#include "scene.h"

class MainScene : public Demx::Scene
{
	Demx::Camera* camera;
	Demx::ParticleSystem* fire;
	Demx::Light* pPointLight;
	Demx::SpotLight* sl;
	Demx::DirectionalLight* dl;
	Demx::SkyBox* sky;

	Demx::Entity* cube;

	GLfloat angle;
	GLfloat rot;

public:
	MainScene( GLvoid );
	~MainScene( GLvoid );

	GL_HRESULT Init();
	GL_HRESULT Loop( GLfloat timeDelta );
	GLvoid Input( MESSAGE msg, PARAM param );
};

