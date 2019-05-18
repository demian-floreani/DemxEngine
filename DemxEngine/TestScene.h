#pragma once

#include "scene.h"

class TestScene : public Demx::Scene
{
	Demx::Camera* camera;

	Demx::Entity* cube1;

	Demx::SkyBox* sky;

	GLfloat rot;

	int i;
public:
	TestScene(void);
	~TestScene(void);

	GL_HRESULT Init();
	GL_HRESULT Loop( GLfloat timeDelta );
	GLvoid Input( MESSAGE msg, PARAM param );
};

