#pragma once
#include "scene.h"

class ParticleScene : public Demx::Scene
{
	Demx::Camera* camera;

	Demx::Entity* cube;

	Demx::SpotLight* sl;
	//Demx::DirectionalLight* dl;

	Demx::PointLight* lightPost1;
	Demx::PointLight* lightPost2;

	GLfloat angle;

	Demx::SkyBox* sky;

public:
	ParticleScene(void);
	~ParticleScene(void);

	GL_HRESULT Init();
	GL_HRESULT Loop( GLfloat timeDelta );
	GLvoid Input( MESSAGE msg, PARAM param );
};