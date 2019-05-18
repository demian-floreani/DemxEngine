#include "stdafx.h"

#include "Renderer.h"

#include "Engine.h"
#include "ModelManager.h"

// Draw the GL scene
//
// @params none
// @return void
//
GL_HRESULT Demx::Renderer::draw()									
{
	//======================================================
	//	Render directional light shadow depth textures
	//======================================================
	//Demx::Shader* depthShader = shaderManager->useShaderProgram("depth");

	setShader( "depth" );

	for(	std::vector<Demx::DirectionalLight*>::const_iterator light = pEngine->lights->directionalLights.begin();
			light != pEngine->lights->directionalLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			drawShadowMapTexture( (*light)->shadow );
		}
	}

	//======================================================
	//	Render spot light shadow depth textures
	//======================================================
	for(	std::vector<Demx::SpotLight*>::const_iterator light = pEngine->lights->spotLights.begin();
			light != pEngine->lights->spotLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			drawShadowMapTexture( (*light)->shadow );
		}
	}

	//======================================================
	//	Render point light shadow depth textures
	//======================================================
	//Demx::Shader* depthCubeShader = shaderManager->useShaderProgram("depth_cube");

	setShader( "depth_cube" );

	for(	std::vector<Demx::PointLight*>::const_iterator light = pEngine->lights->pointLights.begin();
			light != pEngine->lights->pointLights.end();
			++light )
	{
		if( (*light)->hasShadow() == GL_TRUE )
		{
			glClearColor( FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX );
			glCullFace( GL_FRONT );

			program->setUniform( "lightPos", (*light)->position );

			glViewport( 0, 0, (*light)->shadow->resolution, (*light)->shadow->resolution );

			for( GLenum i = 0; i < 6; ++i )
			{
				drawShadowCubeMapTexture( (*light)->shadow, i );
			}
		}
	}

	glViewport( 0, 0, width, height );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	if( debug )
	{
		Demx::Shader* basicProgram = shaderManager->useShaderProgram("basic");
		basicProgram->setUniform( "projectionMatrix", projectionMatrix );
		basicProgram->setUniform( "viewMatrix", viewMatrix );
		basicProgram->setUniform( "color", VEC4( 1.0f, 1.0f, 1.0f, 1.0f ) );

		for(	std::vector<Demx::Entity*>::const_iterator entity = pEngine->entities.begin(); 
				entity != pEngine->entities.end(); 
				++entity)
		{
			basicProgram->setUniform( "modelMatrix", MAT4(1) );
			(*entity)->box.drawLines();
		}

		for(	std::vector<Terrain*>::const_iterator entity = pEngine->terrains.begin(); 
				entity != pEngine->terrains.end(); 
				++entity)
		{
			basicProgram->setUniform( "modelMatrix", MAT4(1) );
			(*entity)->box.drawLines();
		}

		for(	std::vector<Demx::WaveSimultation*>::const_iterator entity = pEngine->fluidSimultations.begin(); 
				entity != pEngine->fluidSimultations.end(); 
				++entity)
		{
			basicProgram->setUniform( "modelMatrix", MAT4(1) );
			(*entity)->box.drawLines();
		}

		shaderManager->stopProgram();
	}

	// skybox
	if( pEngine->skybox != nullptr )
	{
		pEngine->skybox->setPosition( cam->getPosition() );

		Demx::Shader* skyboxProgram = shaderManager->useShaderProgram( "skybox" );

		skyboxProgram->setUniform( "projectionViewModelMatrix", projViewMatrix * pEngine->skybox->modelMatrix );
		skyboxProgram->setUniform( "gSampler", 0 );
		skyboxProgram->setUniform( "color", pEngine->skybox->color );

		// render sky box
		pEngine->skybox->cubeMapTexture->bindTexture();
		glBindVertexArray ( pEngine->skybox->VAO );
		glDrawArrays (GL_TRIANGLES, 0, 36);

		shaderManager->stopProgram();
	}

	if( !pEngine->fluidSimultations.empty() )
	{
		Demx::Shader* waveShader = shaderManager->useShaderProgram("wave");
		waveShader->setUniform( "projectionViewMatrix", projViewMatrix );
		waveShader->setLights( pEngine->lights );
	}

	// start main shader program
	//Demx::Shader* program = shaderManager->useShaderProgram("main");
	setShader( "main" );

	program->setUniform( "projectionViewMatrix", projViewMatrix );
	program->setUniform( "eyeWorldPos", cam->getPosition() );
	program->setLights( pEngine->lights );

	// empty render queue
	queue->empty();

	// populate queue
	addObjectsRenderQueue( queue );

	// sort objects
	queue->sort();

	/*==================================
		Draw opaque objects
	==================================*/
	drawRenderQueue( queue->opaque );

	/*==================================
		Draw objects with transparency
	==================================*/
	drawRenderQueue( queue->transparent );

	if( transparentMode() == GL_TRUE )
	{
		disableTransparentMode();
	}

	// Particle System
	if( !pEngine->particleSystems.empty() )
	{
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );   
		glDepthMask( GL_FALSE );
		               
		Demx::Shader* program = shaderManager->useShaderProgram( "particle" );

		program->setUniform( "projectionMatrix", projectionMatrix );
		program->setUniform( "viewMatrix", viewMatrix );

		for(	std::vector<Demx::ParticleSystem*>::const_iterator particleSystem = pEngine->particleSystems.begin(); 
				particleSystem != pEngine->particleSystems.end(); 
				++particleSystem )
		{	
			// frustum culling
			if( camFrustum->AABBIntersect( (*particleSystem)->box ) == OUTSIDE_VIEW )
			{
				//std::cout << "out";
				continue;
			}

			// update particles
			(*particleSystem)->update( timePassed );
			
			// update AABB
			(*particleSystem)->setupAABB( (*particleSystem)->instancePositions );

			// load material
			Demx::Material& material = this->materialsMap.find( (*particleSystem)->material )->second;
			program->setUniform( "numTextures", material.getTextures().size() );
		
			GLenum j = 0;
			for(auto const &entry : material.getTextures()) 
			{
				texturesMap.find( entry.first )->second.bindTexture( j );
				program->setUniform(predefinedTextureContributions[j], entry.second);
				++j;
			}

			// render
			(*particleSystem)->render();
		}

		shaderManager->stopProgram();

		glDepthMask( GL_TRUE );
		glDisable( GL_BLEND );
	}

	// skybox
	/*if( pEngine->skybox != nullptr )
	{
		pEngine->skybox->setPosition( cam->getPosition() );

		Demx::Shader* skyboxProgram = shaderManager->useShaderProgram( "skybox" );

		skyboxProgram->setUniform("projectionViewModelMatrix", projViewMatrix * pEngine->skybox->modelMatrix );
		skyboxProgram->setUniform("gSampler", 0);

		// render sky box
		pEngine->skybox->cubeMapTexture->bindTexture();
		glBindVertexArray ( pEngine->skybox->VAO );
		glDrawArrays (GL_TRIANGLES, 0, 36);

		shaderManager->stopProgram();
	}*/

	if(debug)
	{
		GLenum err = GL_NO_ERROR;
		if( ( err = glGetError() ) == GL_NO_ERROR )
		{
			return GL_OK;
		}
		else
		{
			std::cout << err << std::endl;
			return GL_FAIL;
		}		
	}

	return GL_OK;
}