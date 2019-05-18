#pragma once
#include "scene.h"

class Scene2 : public Demx::Scene
{
	Demx::Camera* camera;

	Demx::Entity* cube1;

	Demx::SkyBox* sky;

	GLfloat rot;

	int i;
public:
	Scene2(void);
	~Scene2(void);

	GL_HRESULT Init();
	GL_HRESULT Loop( GLfloat timeDelta );
	GLvoid Input( MESSAGE msg, PARAM param );
};

