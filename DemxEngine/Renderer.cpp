#include "stdafx.h"

#include "Terrain.h" 

#include "Renderer.h"

#include "ModelManager.h"
#include "Engine.h"

#include <fstream>
#include <iostream>

Demx::Renderer::Renderer( Engine* engine )
{
	this->pEngine = engine;

	modelManager = new ModelManager();
	shaderManager = new Demx::ShaderManager();

	camFrustum = new Demx::Frustum();
	queue = new Demx::RenderQueue();

	program = nullptr;

	transparent = GL_FALSE;

	frames = 0;
	lastTime = 0;
	timeElapsed = 0.0f;
	debug = GL_FALSE;
	savedVAO = 0;

	//set bias matrix
	biasMatrix[0] = VEC4( 0.5, 0.0, 0.0, 0.0 );
	biasMatrix[1] = VEC4( 0.0, 0.5, 0.0, 0.0 );
	biasMatrix[2] = VEC4( 0.0, 0.0, 0.5, 0.0 );
	biasMatrix[3] = VEC4( 0.5, 0.5, 0.5, 1.0 );
}

Demx::Renderer::~Renderer(GLvoid)
{
	cleanup();
	delete this->modelManager;
	delete this->shaderManager;
	delete this->cam;
}

GLvoid Demx::Renderer::setPredefinedShaderAttributes()
{
	predefinedDepthBiasMVP.push_back("depthBiasMVPDL[0]");
	predefinedDepthBiasMVP.push_back("depthBiasMVPDL[1]");
	predefinedDepthBiasMVP.push_back("depthBiasMVPDL[2]");
	predefinedDepthBiasMVP.push_back("depthBiasMVPDL[3]");

	predefinedTextureContributions.push_back("textureContr[0]");
	predefinedTextureContributions.push_back("textureContr[1]");
	predefinedTextureContributions.push_back("textureContr[2]");
}

/*===============================================================
		Resize GL Window
===============================================================*/
GL_HRESULT Demx::Renderer::resize( GLuint width, GLuint height )		
{
	this->width = width;
	this->height = height;

	projectionMatrix = glm::perspective( 45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 200.0f );
	
	return GL_OK;
}

GL_HRESULT Demx::Renderer::createDepthTexture( GLuint &buffer, GLuint &texture, GLsizei resolution )
{
	// The framebuffer regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers( 1, &buffer );
	glBindFramebuffer( GL_FRAMEBUFFER, buffer );

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer( GL_NONE );

	GLboolean hresult = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return hresult;
}

GL_HRESULT Demx::Renderer::createCubeDepthTexture( GLuint& buffer, GLuint& texture, GLuint& cubeMap, GLsizei resolution )
{
	// The framebuffer regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers( 1, &buffer );

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (GLenum i = 0 ; i < 6 ; ++i) 
	{
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, resolution, resolution, 0, GL_RED, GL_FLOAT, NULL );
    }

	glBindFramebuffer( GL_FRAMEBUFFER, buffer );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
	
	// Disable writes to the color buffer
    glDrawBuffer(GL_NONE);

    // Disable reads from the color buffer
    glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	GLboolean hresult = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return hresult;
}

GL_HRESULT Demx::Renderer::drawShadowMapTexture( const Demx::Shadow* shadow )
{
	shadow->queue->empty();

	addObjectsShadowRenderQueue( shadow->frustum, shadow->position, shadow->queue );

	shadow->queue->sort();

	// draw to framebuffer
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, shadow->framebuffer );
	
	glViewport( 0, 0, shadow->resolution, shadow->resolution );

	glCullFace( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	drawShadowRenderQueue( shadow->depthViewProjMatrix, shadow->queue->opaque );
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return GL_OK;
}

GL_HRESULT Demx::Renderer::drawShadowCubeMapTexture( const Demx::Shadow* shadow, GLenum textarget )
{
	shadow->queue[textarget].empty();

	addObjectsShadowRenderQueue( &shadow->frustum[textarget], shadow->position, &shadow->queue[textarget] );

	shadow->queue->sort();

	// draw to framebuffer
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, shadow->framebuffer );
	
	glFramebufferTexture2D(		GL_DRAW_FRAMEBUFFER, 
								GL_COLOR_ATTACHMENT0, 
								GL_TEXTURE_CUBE_MAP_POSITIVE_X + textarget, 
								shadow->cubeMap, 
								0 );

	glDrawBuffer( GL_COLOR_ATTACHMENT0 );

//	GLint objectType;
//	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE, &objectType);

	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT ); 

	drawShadowRenderQueue( &shadow->depthViewProjMatrix[textarget], shadow->queue[textarget].opaque );

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return GL_OK;
}

GLvoid Demx::Renderer::setShader( const std::string& shader )
{
	program = shaderManager->useShaderProgram( shader );
}

GLvoid Demx::Renderer::setRenderTexture( Demx::Shader* shader, const std::unordered_map<std::string, GLfloat>& textures )
{
	shader->setUniform( "numTextures", textures.size() );
		
	GLenum i = 0;
	for(auto const &entry :textures) 
	{
		texturesMap.find(entry.first)->second.bindTexture(i);
		shader->setUniform(predefinedTextureContributions[i], entry.second);
		++i;
	}
}


GL_HRESULT Demx::Renderer::setRenderMaterial( Demx::Shader* shader, const Demx::Material& material )
{
	setRenderTexture( shader, material.getTextures() );
	shader->setUniform( "specularColor", material.getSpecularColor() );
	shader->setUniform( "phongExp", material.getPhongExp() );
	shader->setUniform( "transparency", material.getTransparency() );

	return GL_OK;
}

GLvoid Demx::Renderer::setTransparentMode( GLboolean isTransparent )
{
	if( isTransparent == GL_TRUE )
	{
		if( transparentMode() == GL_FALSE )
		{
			enableTransparentMode();
		}
	}
	else
	{
		if( transparentMode() == GL_TRUE )
		{
			disableTransparentMode();
		}
	}
}

GLboolean Demx::Renderer::transparentMode()
{
	return transparent;
}

GLvoid Demx::Renderer::enableTransparentMode()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
	glDisable( GL_CULL_FACE );
	glDepthMask( GL_FALSE );

	transparent = GL_TRUE;
}

GLvoid Demx::Renderer::disableTransparentMode()
{
	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );

	transparent = GL_FALSE;
}

GLvoid Demx::Renderer::bindVAO( GLuint VAO )
{
	if( savedVAO != VAO )
	{
		glBindVertexArray( VAO );
		savedVAO = VAO;
	}
}

GLvoid Demx::Renderer::addObjectsRenderQueue( Demx::RenderQueue* queue )
{
	for(	std::vector<Demx::Entity*>::const_iterator entity = pEngine->entities.begin(); 
			entity != pEngine->entities.end(); 
			++entity )
	{
		// frustum culling
		if( camFrustum->AABBIntersect( (*entity)->box ) == OUTSIDE_VIEW )
		{
			continue;
		}
		
		// set distance to camera
		(*entity)->distance = glm::distance( (*entity)->box.getCenter(), cam->getPosition() ); 

		// add entity to render queue
		queue->push( (*entity) );
	}

	for(	std::vector<Terrain*>::const_iterator entity = pEngine->terrains.begin(); 
			entity != pEngine->terrains.end(); 
			++entity )
	{
		// set distance to camera
		(*entity)->distance = FLT_MAX;

		// add entity to render queue
		queue->push( (*entity) );
	}

	for(	std::vector<Demx::WaveSimultation*>::const_iterator wave = pEngine->fluidSimultations.begin(); 
			wave != pEngine->fluidSimultations.end(); 
			++wave )
	{
		// frustum culling
		if( camFrustum->AABBIntersect( (*wave)->box ) == OUTSIDE_VIEW )
		{
			continue;
		}

		// set distance to camera
		(*wave)->distance = glm::distance( (*wave)->box.getCenter(), cam->getPosition() ); 

		// add entity to render queue
		queue->push( (*wave) );
	}
}

GLvoid Demx::Renderer::addObjectsShadowRenderQueue( const Demx::Frustum* frustum, const VEC3& pos, Demx::RenderQueue* queue )
{
	for(	std::vector<Demx::Entity*>::const_iterator entity = pEngine->entities.begin(); 
			entity != pEngine->entities.end(); 
			++entity )
	{
		if( (*entity)->castShadow == GL_FALSE )
			continue;

		// frustum culling
		if( frustum->AABBIntersect( (*entity)->box ) == OUTSIDE_VIEW )
		{
			continue;
		}
		
		// set distance to camera
		(*entity)->distance = glm::distance( (*entity)->box.getCenter(), pos ); 

		// add entity to render queue
		queue->pushOpaque( (*entity) );
	}

	for(	std::vector<Terrain*>::const_iterator entity = pEngine->terrains.begin(); 
			entity != pEngine->terrains.end(); 
			++entity )
	{
		if( (*entity)->castShadow == GL_FALSE )
			continue;

		// set distance to camera
		(*entity)->distance = FLT_MAX;

		// add entity to render queue
		queue->pushOpaque( (*entity) );
	}

	for(	std::vector<Demx::WaveSimultation*>::const_iterator entity = pEngine->fluidSimultations.begin(); 
			entity != pEngine->fluidSimultations.end(); 
			++entity )
	{
		if( (*entity)->castShadow == GL_FALSE )
			continue;

		// frustum culling
		if( frustum->AABBIntersect( (*entity)->box ) == OUTSIDE_VIEW )
		{
			continue;
		}

		// set distance to camera
		(*entity)->distance = glm::distance( (*entity)->box.getCenter(), pos ); 

		// add entity to render queue
		queue->pushOpaque( (*entity) );
	}
}

GLvoid Demx::Renderer::drawRenderQueue( const std::deque<Demx::Entity*>& entities )
{
	savedVAO = 0;

	for(	std::deque<Demx::Entity*>::const_iterator entity = entities.begin(); 
			entity != entities.end(); 
			++entity)
	{
		bindVAO( (*entity)->VAO );

		program = shaderManager->useShaderProgram( (*entity)->shader );

		(*entity)->onStartDraw( program );

		for(	std::vector<Demx::DirectionalLight*>::const_iterator light = pEngine->lights->directionalLights.begin();
				light != pEngine->lights->directionalLights.end();
				++light )
		{
			if( (*light)->hasShadow() == GL_TRUE )
			{
				program->setUniform(	predefinedDepthBiasMVP[ (*light)->shadow->depthIndex ], 
										biasMatrix *
										(*(*light)->shadow->depthViewProjMatrix) * 
										(*entity)->getModelMatrix() );
			}
		}

		for(	std::vector<Demx::SpotLight*>::const_iterator light = pEngine->lights->spotLights.begin();
				light != pEngine->lights->spotLights.end();
				++light )
		{
			if( (*light)->hasShadow() == GL_TRUE )
			{
				program->setUniform(	predefinedDepthBiasMVP[ (*light)->shadow->depthIndex ], 
										biasMatrix *
										(*(*light)->shadow->depthViewProjMatrix) * 
										(*entity)->getModelMatrix() );
			}
		}

		program->setUniform( "modelMatrix", (*entity)->getModelMatrix() );	
		program->setUniform( "normalMatrix", (*entity)->getNormalMatrix() );

		if( (*entity)->isModel() )
		{
			std::vector<Demx::Material>* materials = modelManager->getModelMaterials( (*entity)->model );

			GLModel* model = (*entity)->glModel;

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model->IB );

			for( std::size_t i = 0; i < model->meshes.size(); ++i )
			{
				Demx::Material& material = materials->at( model->meshes[i].materialIndex );				
				setTransparentMode( material.isTransparent() );
				setRenderMaterial( program, material );

				glDrawElements(		GL_TRIANGLES, 
									model->meshes[i].numIndices, 
									GL_UNSIGNED_INT, 
									(const GLvoid*) ( model->meshes[i].startIndices * sizeof( GLuint ) ) );
			}
		}
		else
		{
			Demx::Material& material = this->materialsMap.find( (*entity)->material )->second;
			setTransparentMode( material.isTransparent() );
			setRenderMaterial( program, material );

			if( (*entity)->isIndexed() )
			{
				glDrawElements(		(*entity)->drawType, 
									(*entity)->indices.size(),
									GL_UNSIGNED_INT, 
									(const GLvoid*) 0 );
			}
			else
			{
				glDrawArrays( (*entity)->drawType, 0, (*entity)->vertices.size() );
			}
		}

		(*entity)->onEndDraw();
	}
}

GLvoid Demx::Renderer::drawShadowRenderQueue( const MAT4* depthMatrix, const std::deque<Demx::Entity*>& entities )
{
	savedVAO = 0;

	for(	std::deque<Demx::Entity*>::const_iterator entity = entities.begin(); 
			entity != entities.end(); 
			++entity)
	{
		bindVAO( (*entity)->VAO );

		(*entity)->onStartDraw( program );

		program->setUniform( "depthMVP",	(*depthMatrix) * 
											(*entity)->getModelMatrix() );

		program->setUniform( "model", (*entity)->getModelMatrix() );

		if( (*entity)->isModel() )
		{
			std::vector<Demx::Material>* materials = modelManager->getModelMaterials( (*entity)->model );

			GLModel* model = (*entity)->glModel;

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model->IB );

			for( std::size_t i = 0; i < model->meshes.size(); ++i )
			{
				Demx::Material& material = materials->at( model->meshes[i].materialIndex );				
				setRenderTexture( program, material.getTextures() );

				glDrawElements(		GL_TRIANGLES, 
									model->meshes[i].numIndices, 
									GL_UNSIGNED_INT, 
									(const GLvoid*) ( model->meshes[i].startIndices * sizeof( GLuint ) ) );
			}
		}
		else
		{
			Demx::Material& material = this->materialsMap.find( (*entity)->material )->second;
			setRenderTexture( program, material.getTextures() );

			if( (*entity)->isIndexed() )
			{
				glDrawElements(		(*entity)->drawType, 
									(*entity)->indices.size(),
									GL_UNSIGNED_INT, 
									0 );
			}
			else
			{
				glDrawArrays( (*entity)->drawType, 0, (*entity)->vertices.size() );
			}
		}

		(*entity)->onEndDraw();
	}
}

GL_HRESULT Demx::Renderer::bindShadowTextures()
{
	// bind light shadow textures
	for(	std::vector<Demx::DirectionalLight*>::const_iterator light = pEngine->lights->directionalLights.begin();
			light != pEngine->lights->directionalLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			glActiveTexture( GL_TEXTURE3 + (*light)->shadow->depthIndex );
			glBindTexture( GL_TEXTURE_2D, (*light)->shadow->depthTexture );
		}
	}

	for(	std::vector<Demx::SpotLight*>::const_iterator light = pEngine->lights->spotLights.begin();
			light != pEngine->lights->spotLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			glActiveTexture( GL_TEXTURE3 + (*light)->shadow->depthIndex );
			glBindTexture( GL_TEXTURE_2D, (*light)->shadow->depthTexture );
		}
	}

	for(	std::vector<Demx::PointLight*>::const_iterator light = pEngine->lights->pointLights.begin();
			light != pEngine->lights->pointLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			glActiveTexture( GL_TEXTURE7 + (*light)->shadow->depthIndex );
			glBindTexture( GL_TEXTURE_CUBE_MAP, (*light)->shadow->cubeMap );
		}
	}

	return GL_OK;
}

GL_HRESULT Demx::Renderer::cleanup()
{
	for( std::size_t i = 0; i < this->modelsUser.size(); i++ )
	{
		glDeleteBuffers(3, modelsUser[i]->VBO );
		glDeleteBuffers(1, &modelsUser[i]->IB );
		glDeleteVertexArrays( 1, &modelsUser[i]->VAO );
		delete modelsUser[i];
	}

	for(auto &ent : this->texturesMap) 
	{
		ent.second.releaseTexture();
	}

	return GL_OK;
}