#include "stdafx.h"

#include "Renderer.h"

#include "Engine.h"
#include "ModelManager.h"
#include "Engine.h"
#include "ModelLoader.h"

#include <time.h>



/*===============================================================
		initialize scene
===============================================================*/
GL_HRESULT Demx::Renderer::init()
{
	glewInit();

	// if no camera has been set exit.
	if( cam == nullptr )
	{
		return GL_FAIL;
	}
     
	srand( (unsigned int)time(NULL) );

	glShadeModel( GL_SMOOTH );							
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glClearDepth(1.0f);									
	glEnable( GL_DEPTH_TEST );							
	glDepthFunc( GL_LEQUAL );								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glDisable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_CULL_FACE );

	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0f, 1.0f );

	GLuint depthIndex = 0;
	for(	std::vector<Demx::DirectionalLight*>::const_iterator light = pEngine->lights->directionalLights.begin();
			light != pEngine->lights->directionalLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			(*light)->setShadowDepthIndex( depthIndex++ );

			if(	createDepthTexture(	(*light)->shadow->framebuffer,
									(*light)->shadow->depthTexture,
									(*light)->shadow->resolution ) != GL_OK )
			{
				return GL_FAIL;
			}
		}
	}

	for(	std::vector<Demx::SpotLight*>::const_iterator light = pEngine->lights->spotLights.begin();
			light != pEngine->lights->spotLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			(*light)->setShadowDepthIndex( depthIndex++ );

			if(	createDepthTexture(	(*light)->shadow->framebuffer,
									(*light)->shadow->depthTexture,
									(*light)->shadow->resolution ) != GL_OK )
			{
				return GL_FAIL;
			}
		}
	}

	depthIndex = 0;
	for(	std::vector<Demx::PointLight*>::const_iterator light = pEngine->lights->pointLights.begin();
			light != pEngine->lights->pointLights.end();
			++light )
	{
		if( (*(*light)).hasShadow() == GL_TRUE )
		{
			(*light)->setShadowDepthIndex( depthIndex++ );

			if( createCubeDepthTexture(	(*light)->shadow->framebuffer,
										(*light)->shadow->depthTexture,
										(*light)->shadow->cubeMap,
										(*light)->shadow->resolution ) != GL_OK )
			{
				return GL_FAIL;
			}
		}
	}

	for(	std::vector<Demx::Entity*>::iterator entity = pEngine->entities.begin();
			entity != pEngine->entities.end();
			++entity )
	{
		if( !(*entity)->isModel() )
		{
			(*entity)->VBO = new GLuint[3];

			glGenVertexArrays( 1, &(*entity)->VAO ); 
			glGenBuffers( 3, (*entity)->VBO ); 

			glBindVertexArray( (*entity)->VAO );

			glBindBuffer(GL_ARRAY_BUFFER, (*entity)->VBO[0] );
			glBufferData(GL_ARRAY_BUFFER, (*entity)->vertices.size() * sizeof(GLfloat) * 3, &(*entity)->vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

			glBindBuffer(GL_ARRAY_BUFFER, (*entity)->VBO[1] );
			glBufferData(GL_ARRAY_BUFFER, (*entity)->vertices.size() * sizeof(GLfloat) * 2, &(*entity)->mappings[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	
			glBindBuffer(GL_ARRAY_BUFFER, (*entity)->VBO[2] );
			glBufferData(GL_ARRAY_BUFFER, (*entity)->vertices.size() * sizeof(GLfloat) * 3, &(*entity)->normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

			glBindVertexArray( NULL );

			(*entity)->setupAABB( (*entity)->vertices );

			Demx::Material& material = this->materialsMap.find( (*entity)->material )->second;
			(*entity)->hasTransparency = material.isTransparent();

			if(debug)
			{
				(*entity)->box.transform( (*entity)->modelMatrix );
				(*entity)->box.setupLines();
			}
		}
	}
	
	// load user models
	for(	std::vector<GLModel*>::iterator model = modelsUser.begin();
			model != modelsUser.end();
			++model )
	{
		ModelLoader ml;

		if( ml.processModel( (*model)->path, (*model)->name, modelManager, texturesMap) == GL_FAIL )
		{
			std::cout << "Loading of " << (*model)->path << " failed." << std::endl;
			continue;
		}

		(*model)->VBO = new GLuint[3];

		glGenVertexArrays( 1, &(*model)->VAO ); 
		glGenBuffers( 3, (*model)->VBO );

		glBindVertexArray( (*model)->VAO );

		glBindBuffer(GL_ARRAY_BUFFER, (*model)->VBO[0]);
		glBufferData(GL_ARRAY_BUFFER,  ml.vertices.size() * sizeof(GLfloat) * 3, &ml.vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glBindBuffer(GL_ARRAY_BUFFER, (*model)->VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, ml.vertices.size() * sizeof(GLfloat) * 2, &ml.mappings[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	
		glBindBuffer(GL_ARRAY_BUFFER, (*model)->VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, ml.vertices.size() * sizeof(GLfloat) * 3, &ml.normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glGenBuffers(1, &(*model)->IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*model)->IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ml.indices.size(), &ml.indices[0], GL_STATIC_DRAW);

		glBindVertexArray( NULL );

		(*model)->meshes = ml.meshes;

		// create AABB for model
		Demx::AABB box;
		box.setup( ml.vertices );

		for(	std::vector<Demx::Entity*>::iterator entity = pEngine->entities.begin();
				entity != pEngine->entities.end();
				++entity )
		{
			if( (*entity)->model == (*model)->name )
			{
				(*entity)->VAO = (*model)->VAO;
				(*entity)->glModel = (*model);
				(*entity)->hasTransparency = ml.hasTransparency;

				(*entity)->box = box;

				if( debug )
				{
					(*entity)->box.transform( (*entity)->modelMatrix );
					(*entity)->box.setupLines();
				}
			}
		}
	}
	
	/* terrains */
	for(	std::vector<Terrain*>::const_iterator terrain = pEngine->terrains.begin(); 
			terrain != pEngine->terrains.end(); 
			++terrain )
	{
		if( (*terrain)->init() != GL_OK )
			return GL_FAIL;

		(*terrain)->VBO = new GLuint[4];

		glGenVertexArrays(1, &(*terrain)->VAO);
		glGenBuffers(4, (*terrain)->VBO);

		glBindVertexArray( (*terrain)->VAO ); 

		glBindBuffer(GL_ARRAY_BUFFER, (*terrain)->VBO[0] );
		glBufferData( GL_ARRAY_BUFFER, sizeof(VEC3) * (*terrain)->vertices.size(), &(*terrain)->vertices[0], GL_STATIC_DRAW); 
		glEnableVertexAttribArray(0); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, (*terrain)->VBO[1] );
		glBufferData( GL_ARRAY_BUFFER, sizeof(VEC2) * (*terrain)->mappings.size(), &(*terrain)->mappings[0], GL_STATIC_DRAW); 
		glEnableVertexAttribArray(1); 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, (*terrain)->VBO[2] );
		glBufferData( GL_ARRAY_BUFFER, sizeof(VEC3) * (*terrain)->normals.size(), &(*terrain)->normals[0], GL_STATIC_DRAW); 
		glEnableVertexAttribArray(2); 
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*terrain)->VBO[3]);

		glBufferData(	GL_ELEMENT_ARRAY_BUFFER, 
						sizeof(GLint) * (*terrain)->indices.size(),
						&(*terrain)->indices[0], 
						GL_STATIC_DRAW ); 

		glPrimitiveRestartIndex( (*terrain)->gridX * (*terrain)->gridX );

		(*terrain)->setupAABB( (*terrain)->vertices );

		if(debug)
		{
			(*terrain)->box.transform( (*terrain)->modelMatrix );
			(*terrain)->box.setupLines();
		}

		glBindVertexArray( 0 );
	}

	for(	std::vector<Demx::ParticleSystem*>::const_iterator particleSystem = pEngine->particleSystems.begin(); 
			particleSystem != pEngine->particleSystems.end(); 
			++particleSystem )
	{
		(*particleSystem)->init();

		GLuint* _vbo;
		_vbo = new GLuint[4];

		glGenVertexArrays(1, &(*particleSystem)->VAO); // Create one VAO
		glGenBuffers(4, _vbo); // One VBO for data

		glBindVertexArray((*particleSystem)->VAO);
	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, (*particleSystem)->vertices.size() * sizeof(GLfloat) * 3, &(*particleSystem)->vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, (*particleSystem)->mappings.size() * sizeof(GLfloat) * 2, &(*particleSystem)->mappings[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//instancing
	    glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, (*particleSystem)->particleNum * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
		glBufferData(GL_ARRAY_BUFFER, (*particleSystem)->particleNum * sizeof(GLfloat) * 4, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 0);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);

		(*particleSystem)->vboPositions = _vbo[2];
		(*particleSystem)->vboColors = _vbo[3];

		glBindVertexArray( 0 );
	}

	for(	std::vector<Demx::WaveSimultation*>::const_iterator wave = pEngine->fluidSimultations.begin(); 
			wave != pEngine->fluidSimultations.end(); 
			++wave )
	{
		(*wave)->init();

		(*wave)->VBO = new GLuint[3];

		glGenVertexArrays( 1, &(*wave)->VAO ); 
		glGenBuffers( 3, (*wave)->VBO );

		glBindVertexArray( (*wave)->VAO );

		glBindBuffer(GL_ARRAY_BUFFER, (*wave)->VBO[0] );
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (*wave)->size * 80, &(*wave)->vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, (*wave)->VBO[1] );
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (*wave)->size * 80, &(*wave)->mappings[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		(*wave)->setupAABB( (*wave)->vertices );

		Demx::Material& material = this->materialsMap.find( (*wave)->material )->second;
		(*wave)->hasTransparency = material.isTransparent();

		if(debug)
		{
			(*wave)->box.transform( (*wave)->modelMatrix );
			(*wave)->box.setupLines();
		}

		glBindVertexArray( 0 );
	}

	for( auto it = texturesMap.begin(); it != texturesMap.end(); ++it )
	{
		if( it->second.loadTexture( it->second.path ) != GL_OK )
		{
			std::cout << "Failed to load texture: " << it->second.path << std::endl;
		}
	}

	if( pEngine->skybox != nullptr )
	{
		pEngine->skybox->loadSkybox();
	}

	// shaders
	shaderManager->addShaderProgram("main", "shaders\\main\\shader.vert", "shaders\\main\\shader.frag");
	shaderManager->addShaderProgram("particle", "shaders\\particle\\shader.vert", "shaders\\particle\\shader.frag");
	shaderManager->addShaderProgram("wave", "shaders\\wave\\shader.vert", "shaders\\wave\\shader.frag");
	shaderManager->addShaderProgram("basic", "shaders\\basic\\shader.vert", "shaders\\basic\\shader.frag");
	shaderManager->addShaderProgram("depth", "shaders\\depth\\shader.vert", "shaders\\depth\\shader.frag");
	shaderManager->addShaderProgram("depth_cube", "shaders\\depth_cube\\shader.vert", "shaders\\depth_cube\\shader.frag");
	shaderManager->addShaderProgram("skybox", "shaders\\skybox\\shader.vert", "shaders\\skybox\\shader.frag");
	shaderManager->addShaderProgram("debug", "shaders\\debug\\shader.vert", "shaders\\debug\\shader.frag");

	GLint samplers[3] = { 0, 1, 2 };
	GLint samplersDL[4] = { 3, 4, 5, 6 };
	GLint samplersCubeDL[2] = { 7, 8 };

	Demx::Shader* program = shaderManager->useShaderProgram("main");
	program->setPredefinedLights(pEngine->lights);

	program->setUniform( "gSamplers", 3, samplers );
	program->setUniform( "shadowMapSamplerDL", 4, samplersDL );
	program->setUniform( "shadowMapCubeSampler", 2, samplersCubeDL );

	shaderManager->stopProgram();

	program = shaderManager->useShaderProgram("wave");
	program->setPredefinedLights(pEngine->lights);

	program->setUniform( "gSamplers", 3, samplers );
	program->setUniform( "shadowMapSamplerDL", 4, samplersDL );
	program->setUniform( "shadowMapCubeSampler", 2, samplersCubeDL );

	shaderManager->stopProgram();

	program = shaderManager->useShaderProgram("particle");
	program->setUniform( "gSamplers", 3, samplers );
	shaderManager->stopProgram();

	bindShadowTextures();
	setPredefinedShaderAttributes();

	return GL_OK;								
}
